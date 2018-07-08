#include "InstrumentStatistics.h"

using namespace ordproc;

namespace{
    int sideToIndex(Side side)
    {
        return (Side::BID == side)?(0):(1);
    }

    int statusToIndex(OrderStatus status)
    {
        return (OrderStatus::PENDING == status)?(0):(1);
    }
}

NFQStatistics::NFQStatistics(): nfq_(0)
{
}

NFQStatistics::~NFQStatistics()
{
}

void NFQStatistics::reset()
{
    nfq_ = 0;
}

void NFQStatistics::addNFQ(Side side, QuantityT qty)
{
    nfq_ += (Side::BID == side)?(qty):(-qty);
}

QuantityT NFQStatistics::getNFQ()const
{
    return nfq_;
}

COVStatistics::COVStatistics(): cov_{0.0, 0.0}
{
}

COVStatistics::~COVStatistics()
{
}

void COVStatistics::reset()
{
    cov_ = {0.0, 0.0};
}

void COVStatistics::addCOV(Side side, VolumeT volume)
{
    auto func = [&volume](auto &cont){
        cont += volume;
    };
    func(cov_[sideToIndex(side)]);
}

void COVStatistics::removeCOV(Side side, VolumeT volume)
{
    auto func = [&volume](auto &cont){
        cont -= volume;
    };
    func(cov_[sideToIndex(side)]);
}

VolumeT COVStatistics::getCOV(Side side)const
{
    return cov_[sideToIndex(side)];
}


POVStatistics::POVStatistics():pov_()
{
}

POVStatistics::~POVStatistics()
{
}

void POVStatistics::reset()
{
    pov_ = {};
}

void POVStatistics::addPOV(RequestIdT id, Side side, const OrderParamsT &params)
{
    auto func = [](auto &cont, VolumeT volume, QuantityT qty){
        cont[volume] += qty;
    };
    func(pov_[sideToIndex(side)]
         [statusToIndex(std::get<OrderStatus>(params))],
         std::get<VolumeT>(params),
         std::get<QuantityT>(params));
}

void POVStatistics::removePOV(RequestIdT id, Side side, const OrderParamsT &params)
{
    auto func = [](auto &cont, VolumeT volume, QuantityT qty)
    {
        auto povIt = cont.find(volume);
        if(cont.end() != povIt){
            povIt->second -= qty;
            if(0 == povIt->second)
                cont.erase(povIt);
        }
    };
    func(pov_[sideToIndex(side)]
         [statusToIndex(std::get<OrderStatus>(params))],
         std::get<VolumeT>(params),
         std::get<QuantityT>(params));
}

void POVStatistics::changePOV(RequestIdT id, Side side,
                                     const OrderParamsT &prevParams,
                                     const OrderParamsT &newParams)
{
    if(OrderStatus::PENDING == std::get<OrderStatus>(prevParams) &&
       std::get<OrderStatus>(prevParams) != std::get<OrderStatus>(newParams))
        removePOV(id, side, prevParams);

    auto func = [](auto &cont,
                   VolumeT prevVolume, QuantityT prevQty,
                   VolumeT newVolume, QuantityT newQty)
    {
        auto povIt = cont.find(prevVolume);
        if(cont.end() != povIt){
            povIt->second -= prevQty;
            if(0 == povIt->second)
                cont.erase(povIt);
        }

        if(0.0 < newVolume)
            cont[newVolume] += newQty;
    };
    func(pov_[sideToIndex(side)][statusToIndex(std::get<OrderStatus>(newParams))],
         std::get<VolumeT>(prevParams),
         std::get<QuantityT>(prevParams),
         std::get<VolumeT>(newParams),
         std::get<QuantityT>(newParams));
}

MinMaxVolumeT POVStatistics::getPOV(Side side)const
{
    auto func = [](auto &contPending, auto &contAccepted)->MinMaxVolumeT
    {
        auto maxPend = 0.0;
        if (!contPending.empty()){
            maxPend = contPending.rbegin()->first;
        }
        auto maxAccpt = 0.0, minAccpt = 0.0;
        if (!contAccepted.empty()){
            maxAccpt = contAccepted.rbegin()->first;
            minAccpt = contAccepted.begin()->first;
        }
        return {std::max(0.0, minAccpt), std::max(maxPend, maxAccpt)};
    };
    return func(pov_[sideToIndex(side)][statusToIndex(OrderStatus::PENDING)],
                pov_[sideToIndex(side)][statusToIndex(OrderStatus::NEW)]);
}


