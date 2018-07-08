//
// Created by sam1203 on 7/8/18.
//

#include "StatisticsProcessor.h"

using namespace ordproc;

StatisticsProcessor::StatisticsProcessor(NFQStatistics &nfqStat,
                                         COVStatistics &covStat,
                                         POVStatistics &povStat):
        nfqStat_(nfqStat), covStat_(covStat), povStat_(povStat)
{

}

StatisticsProcessor::~StatisticsProcessor()
{

}

void StatisticsProcessor::onNewOrder(
        RequestIdT id,
        Side side,
        const OrderParamsT &params)
{
    povStat_.addPOV(id, side, params);
}

void StatisticsProcessor::onReplaceOrder(
        RequestIdT newId,
        QuantityT deltaQuantity)
{

}

void StatisticsProcessor::onReplacedOrder(
        RequestIdT oldId,
        const RequestParameters &params)
{
    povStat_.removePOV(oldId, params.side_, {params.status_, params.price_*params.quantity_, params.quantity_});
}


void StatisticsProcessor::onOrderAccepted(RequestIdT id, const RequestParameters &params)
{
    covStat_.addCOV(params.side_, params.price_*params.deltaQuantity_);
    povStat_.changePOV(id,
                    params.side_,
                    {OrderStatus::PENDING, params.price_*params.quantity_, params.quantity_},
                    {params.status_, params.price_*params.quantity_, params.quantity_});
}

void StatisticsProcessor::onOrderRejected(RequestIdT id, const RequestParameters &params)
{
    povStat_.removePOV(id,
                    params.side_,
                    {params.status_, params.price_*params.quantity_, params.quantity_});

}

void StatisticsProcessor::onOrderFilled(RequestIdT id,
                                        const RequestParameters &params,
                                        QuantityT quantityFilled)
{
    nfqStat_.addNFQ(params.side_, quantityFilled);

    if(OrderStatus::REPLACED != params.status_){
        /// add COV, if order was filled without OnRequestAcknowledged() call
        if(OrderStatus::PENDING == params.status_)
            covStat_.addCOV(params.side_, params.price_*params.quantity_);
        covStat_.removeCOV(params.side_, params.price_*quantityFilled);

        QuantityT newQty = params.quantity_ - quantityFilled;
        povStat_.changePOV(id,
                        params.side_,
                        {params.status_, params.price_*params.quantity_, params.quantity_},
                        {OrderStatus::NEW, params.price_*newQty, newQty});
    }
}

void StatisticsProcessor::updateOnBaseOrderFilled(RequestIdT id,
                                                  const RequestParameters &params,
                                                  QuantityT quantityFilled)
{
    if(OrderStatus::PENDING != params.status_)
        covStat_.removeCOV(params.side_, params.price_*quantityFilled);

    QuantityT newQty = params.quantity_ - quantityFilled;

    povStat_.changePOV(id,
                    params.side_,
                    {params.status_, params.price_*params.quantity_, params.quantity_},
                    {params.status_, params.price_*newQty, newQty});
}
