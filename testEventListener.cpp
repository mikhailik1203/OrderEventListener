#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include <boost/test/unit_test.hpp>
#pragma GCC diagnostic pop

#include "EventListener.h"
#include "OrderEventListenerDef.h"

#include <tuple>

using namespace ordproc;

namespace{


}

BOOST_AUTO_TEST_SUITE( EventListenerTest )

    BOOST_AUTO_TEST_CASE( VanillaScenario )
    {
        InstrumentStatistics stat;
        EventListener listenerToTest(stat);

        listenerToTest.OnInsertOrderRequest(1, 'B', 10.0, 10);
        BOOST_CHECK_EQUAL(0, stat.getNFQ());
        BOOST_CHECK_CLOSE(0.0, stat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, stat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        auto minMaxVol = stat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(100.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = stat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());

        listenerToTest.OnRequestAcknowledged(1);
        BOOST_CHECK_EQUAL(0, stat.getNFQ());
        BOOST_CHECK_CLOSE(100.0, stat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, stat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        minMaxVol = stat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(100.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(100.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = stat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());

        listenerToTest.OnInsertOrderRequest(2, 'O', 15.0, 25);
        BOOST_CHECK_EQUAL(0, stat.getNFQ());
        BOOST_CHECK_CLOSE(100.0, stat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, stat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        minMaxVol = stat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(100.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(100.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = stat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(375.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());

        listenerToTest.OnRequestAcknowledged(2);
        BOOST_CHECK_EQUAL(0, stat.getNFQ());
        BOOST_CHECK_CLOSE(100.0, stat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(375.0, stat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        minMaxVol = stat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(100.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(100.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = stat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(375.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(375.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());

        listenerToTest.OnOrderFilled(1, 5);
        BOOST_CHECK_EQUAL(5, stat.getNFQ());
        BOOST_CHECK_CLOSE(50.0, stat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(375.0, stat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        minMaxVol = stat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(50.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(50.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = stat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(375.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(375.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());

        listenerToTest.OnOrderFilled(1, 5);
        BOOST_CHECK_EQUAL(10, stat.getNFQ());
        BOOST_CHECK_CLOSE(0.0, stat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(375.0, stat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        minMaxVol = stat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = stat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(375.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(375.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());

        listenerToTest.OnReplaceOrderRequest(2, 3, 10);
        BOOST_CHECK_EQUAL(10, stat.getNFQ());
        BOOST_CHECK_CLOSE(0.0, stat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(375.0, stat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        minMaxVol = stat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = stat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(375.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(525.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());

        listenerToTest.OnOrderFilled(2, 25);
        BOOST_CHECK_EQUAL(-15, stat.getNFQ());
        BOOST_CHECK_CLOSE(0.0, stat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, stat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        minMaxVol = stat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = stat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(150.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());

        listenerToTest.OnRequestRejected(3);
        BOOST_CHECK_EQUAL(-15, stat.getNFQ());
        BOOST_CHECK_CLOSE(0.0, stat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, stat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        minMaxVol = stat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = stat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
    }

    BOOST_AUTO_TEST_CASE( TestInsertDuplicate ) {
        InstrumentStatistics stat;
        EventListener listenerToTest(stat);

        listenerToTest.OnInsertOrderRequest(1, 'B', 10.0, 10);
        BOOST_CHECK_EQUAL(0, stat.getNFQ());
        BOOST_CHECK_CLOSE(0.0, stat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, stat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        auto minMaxVol = stat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(100.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = stat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());

        /// orders with duplicate id are ignored
        listenerToTest.OnInsertOrderRequest(1, 'O', 2.0, 5);
        BOOST_CHECK_EQUAL(0, stat.getNFQ());
        BOOST_CHECK_CLOSE(0.0, stat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, stat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        minMaxVol = stat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(100.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = stat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
    }

    BOOST_AUTO_TEST_CASE( TestInsertDuplicateAfterFill ) {
        InstrumentStatistics stat;
        EventListener listenerToTest(stat);

        listenerToTest.OnInsertOrderRequest(1, 'B', 10.0, 10);
        listenerToTest.OnOrderFilled(1, 10);
        BOOST_CHECK_EQUAL(10, stat.getNFQ());
        BOOST_CHECK_CLOSE(0.0, stat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, stat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        auto minMaxVol = stat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = stat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());

        /// order processed, because previous order was fullfilled
        listenerToTest.OnInsertOrderRequest(1, 'O', 2.0, 5);
        BOOST_CHECK_EQUAL(10, stat.getNFQ());
        BOOST_CHECK_CLOSE(0.0, stat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, stat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        minMaxVol = stat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = stat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(10.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
    }

    BOOST_AUTO_TEST_CASE( TestInsertReject ) {
        InstrumentStatistics stat;
        EventListener listenerToTest(stat);

        listenerToTest.OnInsertOrderRequest(1, 'B', 10.0, 10);
        BOOST_CHECK_EQUAL(0, stat.getNFQ());
        BOOST_CHECK_CLOSE(0.0, stat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, stat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        auto minMaxVol = stat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(100.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = stat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());

        listenerToTest.OnRequestRejected(1);
        BOOST_CHECK_EQUAL(0, stat.getNFQ());
        BOOST_CHECK_CLOSE(0.0, stat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, stat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        minMaxVol = stat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = stat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
    }

    BOOST_AUTO_TEST_CASE( TestDuplicateReject ) {
        InstrumentStatistics stat;
        EventListener listenerToTest(stat);

        listenerToTest.OnInsertOrderRequest(1, 'B', 10.0, 10);
        BOOST_CHECK_EQUAL(0, stat.getNFQ());
        BOOST_CHECK_CLOSE(0.0, stat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, stat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        auto minMaxVol = stat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(100.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = stat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());

        listenerToTest.OnRequestRejected(1);
        BOOST_CHECK_EQUAL(0, stat.getNFQ());
        BOOST_CHECK_CLOSE(0.0, stat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, stat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        minMaxVol = stat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = stat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());

        listenerToTest.OnRequestRejected(1);
        BOOST_CHECK_EQUAL(0, stat.getNFQ());
        BOOST_CHECK_CLOSE(0.0, stat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, stat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        minMaxVol = stat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = stat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
    }

    BOOST_AUTO_TEST_CASE( TestRejectForAccepted ) {
        InstrumentStatistics stat;
        EventListener listenerToTest(stat);

        listenerToTest.OnInsertOrderRequest(1, 'B', 10.0, 10);
        BOOST_CHECK_EQUAL(0, stat.getNFQ());
        BOOST_CHECK_CLOSE(0.0, stat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, stat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        auto minMaxVol = stat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(100.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = stat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());

        listenerToTest.OnRequestAcknowledged(1);
        BOOST_CHECK_EQUAL(0, stat.getNFQ());
        BOOST_CHECK_CLOSE(100.0, stat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, stat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        minMaxVol = stat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(100.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(100.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = stat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());

        listenerToTest.OnRequestRejected(1);
        BOOST_CHECK_EQUAL(0, stat.getNFQ());
        BOOST_CHECK_CLOSE(100.0, stat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, stat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        minMaxVol = stat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(100.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(100.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = stat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
    }

    BOOST_AUTO_TEST_CASE( TestInsertAccepted ) {
        InstrumentStatistics stat;
        EventListener listenerToTest(stat);

        listenerToTest.OnInsertOrderRequest(1, 'B', 10.0, 10);
        BOOST_CHECK_EQUAL(0, stat.getNFQ());
        BOOST_CHECK_CLOSE(0.0, stat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, stat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        auto minMaxVol = stat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(100.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = stat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());

        listenerToTest.OnRequestAcknowledged(1);
        BOOST_CHECK_EQUAL(0, stat.getNFQ());
        BOOST_CHECK_CLOSE(100.0, stat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, stat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        minMaxVol = stat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(100.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(100.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = stat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
    }

    BOOST_AUTO_TEST_CASE( TestDuplicateAccepted ) {
        InstrumentStatistics stat;
        EventListener listenerToTest(stat);

        listenerToTest.OnInsertOrderRequest(1, 'B', 10.0, 10);
        BOOST_CHECK_EQUAL(0, stat.getNFQ());
        BOOST_CHECK_CLOSE(0.0, stat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, stat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        auto minMaxVol = stat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(100.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = stat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());

        listenerToTest.OnRequestAcknowledged(1);
        BOOST_CHECK_EQUAL(0, stat.getNFQ());
        BOOST_CHECK_CLOSE(100.0, stat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, stat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        minMaxVol = stat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(100.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(100.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = stat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());

        listenerToTest.OnRequestAcknowledged(1);
        BOOST_CHECK_EQUAL(0, stat.getNFQ());
        BOOST_CHECK_CLOSE(100.0, stat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, stat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        minMaxVol = stat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(100.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(100.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = stat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
    }


    BOOST_AUTO_TEST_CASE( TestInsertReplaced ) {
        InstrumentStatistics stat;
        EventListener listenerToTest(stat);

        listenerToTest.OnInsertOrderRequest(1, 'B', 10.0, 10);
        BOOST_CHECK_EQUAL(0, stat.getNFQ());
        BOOST_CHECK_CLOSE(0.0, stat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, stat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        auto minMaxVol = stat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(100.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = stat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());

        listenerToTest.OnReplaceOrderRequest(1, 2, 20);
        BOOST_CHECK_EQUAL(0, stat.getNFQ());
        BOOST_CHECK_CLOSE(0.0, stat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, stat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        minMaxVol = stat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(300.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = stat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
    }

    BOOST_AUTO_TEST_CASE( TestDuplicateReplaced ) {
        InstrumentStatistics stat;
        EventListener listenerToTest(stat);

        listenerToTest.OnInsertOrderRequest(1, 'B', 10.0, 10);
        BOOST_CHECK_EQUAL(0, stat.getNFQ());
        BOOST_CHECK_CLOSE(0.0, stat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, stat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        auto minMaxVol = stat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(100.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = stat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());

        listenerToTest.OnReplaceOrderRequest(1, 2, 20);
        BOOST_CHECK_EQUAL(0, stat.getNFQ());
        BOOST_CHECK_CLOSE(0.0, stat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, stat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        minMaxVol = stat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(300.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = stat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());

        /// duplicate order is ignored
        listenerToTest.OnReplaceOrderRequest(1, 2, 20);
        BOOST_CHECK_EQUAL(0, stat.getNFQ());
        BOOST_CHECK_CLOSE(0.0, stat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, stat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        minMaxVol = stat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(300.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = stat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
    }

    BOOST_AUTO_TEST_CASE( TestReplaceReplaced ) {
        InstrumentStatistics stat;
        EventListener listenerToTest(stat);

        listenerToTest.OnInsertOrderRequest(1, 'B', 10.0, 10);
        BOOST_CHECK_EQUAL(0, stat.getNFQ());
        BOOST_CHECK_CLOSE(0.0, stat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, stat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        auto minMaxVol = stat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(100.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = stat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());

        listenerToTest.OnReplaceOrderRequest(1, 2, 20);
        BOOST_CHECK_EQUAL(0, stat.getNFQ());
        BOOST_CHECK_CLOSE(0.0, stat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, stat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        minMaxVol = stat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(300.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = stat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());

        listenerToTest.OnReplaceOrderRequest(2, 3, -10);
        BOOST_CHECK_EQUAL(0, stat.getNFQ());
        BOOST_CHECK_CLOSE(0.0, stat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, stat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        minMaxVol = stat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(300.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = stat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());

    }

    BOOST_AUTO_TEST_CASE( TestReplaceReplaced_RejectReplaced ) {
        InstrumentStatistics stat;
        EventListener listenerToTest(stat);

        listenerToTest.OnInsertOrderRequest(1, 'B', 10.0, 10);
        BOOST_CHECK_EQUAL(0, stat.getNFQ());
        BOOST_CHECK_CLOSE(0.0, stat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, stat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        auto minMaxVol = stat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(100.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = stat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());

        listenerToTest.OnReplaceOrderRequest(1, 2, 20);
        BOOST_CHECK_EQUAL(0, stat.getNFQ());
        BOOST_CHECK_CLOSE(0.0, stat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, stat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        minMaxVol = stat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(300.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = stat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());

        listenerToTest.OnReplaceOrderRequest(2, 3, -10);
        BOOST_CHECK_EQUAL(0, stat.getNFQ());
        BOOST_CHECK_CLOSE(0.0, stat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, stat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        minMaxVol = stat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(300.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = stat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());

        listenerToTest.OnRequestRejected(2);
        BOOST_CHECK_EQUAL(0, stat.getNFQ());
        BOOST_CHECK_CLOSE(0.0, stat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, stat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        minMaxVol = stat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(200.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = stat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());

    }

    BOOST_AUTO_TEST_CASE( TestReplaceReplaced_Fill ) {
        InstrumentStatistics stat;
        EventListener listenerToTest(stat);

        listenerToTest.OnInsertOrderRequest(1, 'B', 10.0, 10);
        BOOST_CHECK_EQUAL(0, stat.getNFQ());
        BOOST_CHECK_CLOSE(0.0, stat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, stat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        auto minMaxVol = stat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(100.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = stat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());

        listenerToTest.OnReplaceOrderRequest(1, 2, 20);
        BOOST_CHECK_EQUAL(0, stat.getNFQ());
        BOOST_CHECK_CLOSE(0.0, stat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, stat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        minMaxVol = stat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(300.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = stat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());

        listenerToTest.OnReplaceOrderRequest(2, 3, -10);
        BOOST_CHECK_EQUAL(0, stat.getNFQ());
        BOOST_CHECK_CLOSE(0.0, stat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, stat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        minMaxVol = stat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(300.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = stat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());

        listenerToTest.OnOrderFilled(1, 5);
        BOOST_CHECK_EQUAL(5, stat.getNFQ());
        BOOST_CHECK_CLOSE(50.0, stat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, stat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        minMaxVol = stat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(50.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(250.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = stat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());

    }

    BOOST_AUTO_TEST_CASE( TestFillAfterReplaced ) {
        InstrumentStatistics stat;
        EventListener listenerToTest(stat);

        listenerToTest.OnInsertOrderRequest(1, 'B', 10.0, 10);
        BOOST_CHECK_EQUAL(0, stat.getNFQ());
        BOOST_CHECK_CLOSE(0.0, stat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, stat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        auto minMaxVol = stat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(100.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = stat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());

        listenerToTest.OnReplaceOrderRequest(1, 2, 20);
        BOOST_CHECK_EQUAL(0, stat.getNFQ());
        BOOST_CHECK_CLOSE(0.0, stat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, stat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        minMaxVol = stat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(300.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = stat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());

        listenerToTest.OnRequestAcknowledged(1);
        listenerToTest.OnRequestAcknowledged(2);
        BOOST_CHECK_EQUAL(0, stat.getNFQ());
        BOOST_CHECK_CLOSE(300.0, stat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, stat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        minMaxVol = stat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(300.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(300.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = stat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());

        listenerToTest.OnOrderFilled(1, 5);
        BOOST_CHECK_EQUAL(5, stat.getNFQ());
        BOOST_CHECK_CLOSE(250.0, stat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, stat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        minMaxVol = stat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(250.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(250.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = stat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());

        listenerToTest.OnOrderFilled(1, 3);
        BOOST_CHECK_EQUAL(8, stat.getNFQ());
        BOOST_CHECK_CLOSE(220.0, stat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, stat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        minMaxVol = stat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(220.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(220.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = stat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());

    }

BOOST_AUTO_TEST_SUITE_END()