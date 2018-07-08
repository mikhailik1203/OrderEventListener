#ifndef ORDEREVENTLISTENER_INSTRUMENTSTATISTICS_H
#define ORDEREVENTLISTENER_INSTRUMENTSTATISTICS_H

#include "OrderEventListenerDef.h"

#include <map>
#include <array>

namespace ordproc{

    /**
     * *Statistics classes encapsulate storage for statistic's parameters NFQ, COV and POV
     */
    class NFQStatistics {
    public:
        NFQStatistics();
        ~NFQStatistics();

        void reset();

        QuantityT getNFQ()const;
        void addNFQ(Side side, QuantityT qty);

    private:
        QuantityT nfq_;
    };


    class COVStatistics {
    public:
        COVStatistics();
        ~COVStatistics();

        void reset();

        VolumeT getCOV(Side side)const;
        void addCOV(Side side, VolumeT volume);
        void removeCOV(Side side, VolumeT volume);

    private:
        std::array<VolumeT, SIDE_COUNT> cov_;
    };

    class POVStatistics {
    public:
        POVStatistics();
        ~POVStatistics();

        void reset();

        MinMaxVolumeT getPOV(Side side)const;
        void addPOV(RequestIdT id, Side side, const OrderParamsT &params);
        void removePOV(RequestIdT id, Side side, const OrderParamsT &params);
        void changePOV(RequestIdT id, Side side,
                       const OrderParamsT &prevParams,
                       const OrderParamsT &newParams);
    private:
        typedef std::map<VolumeT, QuantityT> POVT;
        std::array<std::array<POVT, SIDE_COUNT>, STATUS_COUNT> pov_;
    };

}


#endif //ORDEREVENTLISTENER_INSTRUMENTSTATISTICS_H
