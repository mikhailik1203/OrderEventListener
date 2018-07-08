#ifndef ORDEREVENTLISTENER_ORDEREVENTLISTENERDEF_H
#define ORDEREVENTLISTENER_ORDEREVENTLISTENERDEF_H

#include <tuple>

namespace ordproc{

    typedef int RequestIdT;
    const RequestIdT INVALID_REQID = 0;
    typedef int QuantityT;
    typedef double PriceT;
    const PriceT INVALID_PRICE = 0.0;
    typedef double VolumeT;

    typedef std::tuple<VolumeT, VolumeT> MinMaxVolumeT;

    enum class Side: char{
        BID = 'B',
        OFFER = 'O'
    };
    const int SIDE_COUNT = 2;

    enum class OrderStatus{
        PENDING,
        NEW,
        REPLACED
    };
    const int STATUS_COUNT = 2;

    struct RequestParameters{
        PriceT price_ = 0.0;
        QuantityT quantity_ = 0;
        QuantityT deltaQuantity_ = 0;
        RequestIdT childReqId_ = INVALID_REQID;
        RequestIdT baseReqId_ = INVALID_REQID;
        OrderStatus status_ = OrderStatus::PENDING;
        Side side_;
    };

    typedef std::tuple<OrderStatus, VolumeT, QuantityT> OrderParamsT;

}

#endif //ORDEREVENTLISTENER_ORDEREVENTLISTENERDEF_H
