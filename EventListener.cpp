
#include "EventListener.h"

using namespace ordproc;

namespace {
    const size_t REQUEST_AMOUNT = 1000;
}

EventListener::EventListener(StatisticsProcessor &proc):
        proc_(proc)
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
    proc_.onNewOrder(id, static_cast<Side>(side), {OrderStatus::PENDING, price*quantity, quantity});
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
    proc_.onNewOrder(newId, it->second.side_, {OrderStatus::PENDING, it->second.price_*newQty, newQty});
    it->second.childReqId_ = newId;
}

void EventListener::replaceBaseRequests(
        int id)
{
    auto it = requests_.find(id);
    if(requests_.end() != it){
        proc_.onReplacedOrder(id, it->second);

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

    it->second.status_ = OrderStatus::NEW;

    if(INVALID_REQID != it->second.baseReqId_)
        replaceBaseRequests(it->second.baseReqId_);

    proc_.onOrderAccepted(id, it->second);
}

void EventListener::OnRequestRejected(
        int id)
{
    auto it = requests_.find(id);
    if(requests_.end() == it)
        return;

    if(OrderStatus::PENDING != it->second.status_)
        return;

    proc_.onOrderRejected(id, it->second);
    requests_.erase(it);
}

void EventListener::updateChildRequest(RequestIdT id, QuantityT quantityFilled)
{
    auto it = requests_.find(id);
    if(requests_.end() == it)
        return;

    proc_.updateOnBaseOrderFilled(id, it->second, quantityFilled);
    it->second.quantity_ -= quantityFilled;

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

    proc_.onOrderFilled(id, it->second, quantityFilled);

    it->second.quantity_ -= quantityFilled;
    if(OrderStatus::REPLACED != it->second.status_)
        it->second.status_ = OrderStatus::NEW;

    if(INVALID_REQID != it->second.childReqId_)
        updateChildRequest(it->second.childReqId_, quantityFilled);

    if(0 == it->second.quantity_)
        requests_.erase(it);
}

