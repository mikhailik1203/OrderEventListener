
#ifndef ORDEREVENTLISTENER_EVENTLISTENER_H
#define ORDEREVENTLISTENER_EVENTLISTENER_H

#include "Listener.h"
#include "OrderEventListenerDef.h"
#include "InstrumentStatistics.h"
#include "StatisticsProcessor.h"

#include <unordered_map>
#include <map>
#include <unordered_set>
#include <array>

namespace ordproc{

/**
 * EventListener contains logic of the Order's event processing.
 *      Encapsulates state of the orders.
 *
    State machine for EventListener
    State\Event | OnInsertOrderRequest | OnReplaceOrderRequest(OldId) | OnReplaceOrderRequest(NewId) | OnRequestAcknowledged | OnRequestRejected | OnOrderFilled
-----------------------------------------------------------------------------------------------------------------------------------------------------------------
    N/A         |    Accepted(PENDING) |  Ignored                     |    Accepted(PENDING)         |    Ignored            |  Ignored          |  Ignored
-----------------------------------------------------------------------------------------------------------------------------------------------------------------
    PENDING     |    Ignored           | Accepted(REPLACED)           |    Ignored                   |    Accepted(NEW)      |  Accepted         |  Accepted
-----------------------------------------------------------------------------------------------------------------------------------------------------------------
    NEW         |    Ignored           | Accepted(REPLACED)           |    Ignored                   |    Ignored            |  Ignored          |  Accepted
-----------------------------------------------------------------------------------------------------------------------------------------------------------------
    REPLACED    |    Ignored           | Ignored                      |    Ignored                   |    Ignored            |  Ignored          |  Accepted

   Accepted - event is processed and order is switched to the new state
   Ignored - event is ignored, order is not changed
 */
    class EventListener: public Listener{
    public:
        explicit EventListener(StatisticsProcessor &proc);
        virtual ~EventListener(){}

    public:
        /// implementation of the Listener interface
        virtual void OnInsertOrderRequest(
                int id,
                char side,
                double price,
                int quantity) override;

        virtual void OnReplaceOrderRequest(
                int oldId,
                int newId,
                int deltaQuantity) override;

        virtual void OnRequestAcknowledged(
                int id) override;

        virtual void OnRequestRejected(
                int id) override;

        virtual void OnOrderFilled(
                int id,
                int quantityFilled) override;
    private:
        void updateChildRequest(RequestIdT id, QuantityT quantityFilled);
        void replaceBaseRequests(RequestIdT id);

    public:
        typedef std::unordered_map<RequestIdT, RequestParameters> RequestsT;

    private:
        RequestsT requests_;
        StatisticsProcessor &proc_;
    };


}


#endif //ORDEREVENTLISTENER_EVENTLISTENER_H
