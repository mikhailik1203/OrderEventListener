//
// Created by sam1203 on 7/8/18.
//

#ifndef ORDEREVENTLISTENER_STATISTICSPROCESSOR_H
#define ORDEREVENTLISTENER_STATISTICSPROCESSOR_H

#include "OrderEventListenerDef.h"

#include "InstrumentStatistics.h"

namespace ordproc{
    class StatisticsProcessor {
    public:
        StatisticsProcessor(NFQStatistics &nfqStat,
                            COVStatistics &covStat,
                            POVStatistics &povStat);
        ~StatisticsProcessor();

    public:
        void onNewOrder(
                RequestIdT id,
                Side side,
                const OrderParamsT &params);

        void onReplaceOrder(
                RequestIdT newId,
                QuantityT deltaQuantity);

        void onReplacedOrder(
                RequestIdT oldId,
                const RequestParameters &params);

        void onOrderAccepted(
                RequestIdT id,
                const RequestParameters &params);

        void onOrderRejected(
                RequestIdT id,
                const RequestParameters &params);

        void onOrderFilled(
                RequestIdT id,
                const RequestParameters &params,
                QuantityT quantityFilled);
        void updateOnBaseOrderFilled(
                RequestIdT id,
                const RequestParameters &params,
                QuantityT quantityFilled);

    private:
        NFQStatistics &nfqStat_;
        COVStatistics &covStat_;
        POVStatistics &povStat_;
    };

}



#endif //ORDEREVENTLISTENER_STATISTICSPROCESSOR_H
