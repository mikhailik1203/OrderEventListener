#ifndef ORDEREVENTLISTENER_INSTRUMENTSTATISTICS_H
#define ORDEREVENTLISTENER_INSTRUMENTSTATISTICS_H

#include "OrderEventListenerDef.h"

#include <map>
#include <array>

namespace ordproc{

    /**
     * InstrumentStatistics is the storage for statistic's parameters NFQ, COV and POV
     */
    class InstrumentStatistics {
    public:
        InstrumentStatistics();
        ~InstrumentStatistics();

        void reset();

        MinMaxVolumeT getPOV(Side side)const;
        void addPOV(RequestIdT id, Side side, const OrderParamsT &params);
        void removePOV(RequestIdT id, Side side, const OrderParamsT &params);
        void changePOV(RequestIdT id, Side side,
                       const OrderParamsT &prevParams,
                       const OrderParamsT &newParams);

        VolumeT getCOV(Side side)const;
        void addCOV(Side side, VolumeT volume);
        void removeCOV(Side side, VolumeT volume);

        QuantityT getNFQ()const;
        void addNFQ(Side side, QuantityT qty);

    private:
        typedef std::map<VolumeT, QuantityT> POVT;
        std::array<std::array<POVT, SIDE_COUNT>, STATUS_COUNT> pov_;

        std::array<VolumeT, SIDE_COUNT> cov_;
        QuantityT nfq_;
    };

}


#endif //ORDEREVENTLISTENER_INSTRUMENTSTATISTICS_H
