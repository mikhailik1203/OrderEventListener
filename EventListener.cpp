
#include "EventListener.h"

using namespace ordproc;

namespace {
    const size_t REQUEST_AMOUNT = 1000;
}

EventListener::EventListener(InstrumentStatistics &stat):stat_(stat)
{
    requests_.reserve(REQUEST_AMOUNT);
}

void EventListener::OnInsertOrderRequest(
        int id,
        char side,
        double price,
        int quantity)
{
    /// orders with duplicate id are ignored
    auto it = requests_.find(id);
    if(requests_.end() != it)
        return;

    requests_[id] = {price,
                     quantity,
                     quantity,
                     INVALID_REQID,
                     INVALID_REQID,
                     OrderStatus::PENDING,
                     static_cast<Side>(side)};
    stat_.addPOV(id, static_cast<Side>(side), {OrderStatus::PENDING, price*quantity, quantity});
}

void EventListener::OnReplaceOrderRequest(
        int oldId,
        int newId,
        int deltaQuantity)
{
    auto it = requests_.find(oldId);
    if(requests_.end() == it || OrderStatus::REPLACED == it->second.status_)
        return;

    auto newIt = requests_.find(newId);
    if(requests_.end() != newIt)
        return;

    QuantityT newQty = it->second.quantity_ + deltaQuantity;

    requests_[newId] = {it->second.price_,
                        newQty,
                        deltaQuantity,
                        INVALID_REQID,
                        oldId,
                        OrderStatus::PENDING,
                        it->second.side_};
    stat_.addPOV(newId, it->second.side_, {OrderStatus::PENDING, it->second.price_*newQty, newQty});
    it->second.childReqId_ = newId;
}

void EventListener::replaceBaseRequests(
        int id)
{
    auto it = requests_.find(id);
    if(requests_.end() != it){
        stat_.removePOV(id, it->second.side_, {it->second.status_, it->second.price_*it->second.quantity_, it->second.quantity_});
        it->second.status_ = OrderStatus::REPLACED;

        if(INVALID_REQID != it->second.baseReqId_)
            replaceBaseRequests(it->second.baseReqId_);
    }
}

void EventListener::OnRequestAcknowledged(
        int id)
{
    auto it = requests_.find(id);
    if(requests_.end() == it)
        return;

    /// skip duplicate OrderAccepted
    if(OrderStatus::PENDING != it->second.status_)
        return;

    OrderStatus prevStatus = it->second.status_;
    it->second.status_ = OrderStatus::NEW;

    if(INVALID_REQID != it->second.baseReqId_)
        replaceBaseRequests(it->second.baseReqId_);

    QuantityT deltaQty = it->second.deltaQuantity_;
    stat_.addCOV(it->second.side_, it->second.price_*deltaQty);
    stat_.changePOV(id,
                    it->second.side_,
                    {prevStatus, it->second.price_*it->second.quantity_, it->second.quantity_},
                    {it->second.status_, it->second.price_*it->second.quantity_, it->second.quantity_});
}

void EventListener::OnRequestRejected(
        int id)
{
    auto it = requests_.find(id);
    if(requests_.end() == it)
        return;

    if(OrderStatus::PENDING != it->second.status_)
        return;

    stat_.removePOV(id,
                    it->second.side_,
                    {it->second.status_, it->second.price_*it->second.quantity_, it->second.quantity_});
    requests_.erase(it);
}

void EventListener::updateChildRequest(RequestIdT id, QuantityT quantityFilled)
{
    auto it = requests_.find(id);
    if(requests_.end() == it)
        return;

    QuantityT prevQty = it->second.quantity_;
    it->second.quantity_ -= quantityFilled;

    if(OrderStatus::PENDING != it->second.status_)
        stat_.removeCOV(it->second.side_, it->second.price_*quantityFilled);

    stat_.changePOV(id,
                    it->second.side_,
                    {it->second.status_, it->second.price_*prevQty, prevQty},
                    {it->second.status_, it->second.price_*it->second.quantity_, it->second.quantity_});

    if(INVALID_REQID != it->second.childReqId_)
        updateChildRequest(it->second.childReqId_, quantityFilled);

    if(0 == it->second.quantity_)
        requests_.erase(it);
}

void EventListener::OnOrderFilled(
        int id,
        int quantityFilled)
{
    auto it = requests_.find(id);
    if(requests_.end() == it)
        return;

    QuantityT prevQty = it->second.quantity_;
    OrderStatus prevStatus = it->second.status_;
    it->second.quantity_ -= quantityFilled;

    stat_.addNFQ(it->second.side_, quantityFilled);

    if(OrderStatus::REPLACED != prevStatus){
        it->second.status_ = OrderStatus::NEW;

        /// add COV, if order was filled without OnRequestAcknowledged() call
        if(OrderStatus::PENDING == prevStatus)
            stat_.addCOV(it->second.side_, it->second.price_*prevQty);
        stat_.removeCOV(it->second.side_, it->second.price_*quantityFilled);

        stat_.changePOV(id,
                        it->second.side_,
                        {prevStatus, it->second.price_*prevQty, prevQty},
                        {it->second.status_, it->second.price_*it->second.quantity_, it->second.quantity_});
    }
    if(INVALID_REQID != it->second.childReqId_)
        updateChildRequest(it->second.childReqId_, quantityFilled);

    if(0 == it->second.quantity_)
        requests_.erase(it);
}

