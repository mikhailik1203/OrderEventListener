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
        NFQStatistics nfqStat;
        COVStatistics covStat;
        POVStatistics povStat;
        StatisticsProcessor proc(nfqStat, covStat, povStat);
        EventListener listenerToTest(proc);

        listenerToTest.OnInsertOrderRequest(1, 'B', 10.0, 10);
        BOOST_CHECK_EQUAL(0, nfqStat.getNFQ());
        BOOST_CHECK_CLOSE(0.0, covStat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, covStat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        auto minMaxVol = povStat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(100.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = povStat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());

        listenerToTest.OnRequestAcknowledged(1);
        BOOST_CHECK_EQUAL(0, nfqStat.getNFQ());
        BOOST_CHECK_CLOSE(100.0, covStat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, covStat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        minMaxVol = povStat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(100.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(100.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = povStat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());

        listenerToTest.OnInsertOrderRequest(2, 'O', 15.0, 25);
        BOOST_CHECK_EQUAL(0, nfqStat.getNFQ());
        BOOST_CHECK_CLOSE(100.0, covStat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, covStat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        minMaxVol = povStat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(100.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(100.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = povStat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(375.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());

        listenerToTest.OnRequestAcknowledged(2);
        BOOST_CHECK_EQUAL(0, nfqStat.getNFQ());
        BOOST_CHECK_CLOSE(100.0, covStat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(375.0, covStat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        minMaxVol = povStat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(100.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(100.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = povStat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(375.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(375.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());

        listenerToTest.OnOrderFilled(1, 5);
        BOOST_CHECK_EQUAL(5, nfqStat.getNFQ());
        BOOST_CHECK_CLOSE(50.0, covStat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(375.0, covStat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        minMaxVol = povStat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(50.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(50.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = povStat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(375.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(375.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());

        listenerToTest.OnOrderFilled(1, 5);
        BOOST_CHECK_EQUAL(10, nfqStat.getNFQ());
        BOOST_CHECK_CLOSE(0.0, covStat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(375.0, covStat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        minMaxVol = povStat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = povStat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(375.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(375.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());

        listenerToTest.OnReplaceOrderRequest(2, 3, 10);
        BOOST_CHECK_EQUAL(10, nfqStat.getNFQ());
        BOOST_CHECK_CLOSE(0.0, covStat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(375.0, covStat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        minMaxVol = povStat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = povStat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(375.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(525.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());

        listenerToTest.OnOrderFilled(2, 25);
        BOOST_CHECK_EQUAL(-15, nfqStat.getNFQ());
        BOOST_CHECK_CLOSE(0.0, covStat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, covStat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        minMaxVol = povStat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = povStat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(150.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());

        listenerToTest.OnRequestRejected(3);
        BOOST_CHECK_EQUAL(-15, nfqStat.getNFQ());
        BOOST_CHECK_CLOSE(0.0, covStat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, covStat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        minMaxVol = povStat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = povStat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
    }

    BOOST_AUTO_TEST_CASE( TestInsertDuplicate ) {
        NFQStatistics nfqStat;
        COVStatistics covStat;
        POVStatistics povStat;
        StatisticsProcessor proc(nfqStat, covStat, povStat);
        EventListener listenerToTest(proc);

        listenerToTest.OnInsertOrderRequest(1, 'B', 10.0, 10);
        BOOST_CHECK_EQUAL(0, nfqStat.getNFQ());
        BOOST_CHECK_CLOSE(0.0, covStat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, covStat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        auto minMaxVol = povStat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(100.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = povStat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());

        /// orders with duplicate id are ignored
        listenerToTest.OnInsertOrderRequest(1, 'O', 2.0, 5);
        BOOST_CHECK_EQUAL(0, nfqStat.getNFQ());
        BOOST_CHECK_CLOSE(0.0, covStat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, covStat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        minMaxVol = povStat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(100.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = povStat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
    }

    BOOST_AUTO_TEST_CASE( TestInsertDuplicateAfterFill ) {
        NFQStatistics nfqStat;
        COVStatistics covStat;
        POVStatistics povStat;
        StatisticsProcessor proc(nfqStat, covStat, povStat);
        EventListener listenerToTest(proc);

        listenerToTest.OnInsertOrderRequest(1, 'B', 10.0, 10);
        listenerToTest.OnOrderFilled(1, 10);
        BOOST_CHECK_EQUAL(10, nfqStat.getNFQ());
        BOOST_CHECK_CLOSE(0.0, covStat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, covStat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        auto minMaxVol = povStat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = povStat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());

        /// order processed, because previous order was fullfilled
        listenerToTest.OnInsertOrderRequest(1, 'O', 2.0, 5);
        BOOST_CHECK_EQUAL(10, nfqStat.getNFQ());
        BOOST_CHECK_CLOSE(0.0, covStat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, covStat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        minMaxVol = povStat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = povStat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(10.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
    }

    BOOST_AUTO_TEST_CASE( TestInsertReject ) {
        NFQStatistics nfqStat;
        COVStatistics covStat;
        POVStatistics povStat;
        StatisticsProcessor proc(nfqStat, covStat, povStat);
        EventListener listenerToTest(proc);

        listenerToTest.OnInsertOrderRequest(1, 'B', 10.0, 10);
        BOOST_CHECK_EQUAL(0, nfqStat.getNFQ());
        BOOST_CHECK_CLOSE(0.0, covStat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, covStat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        auto minMaxVol = povStat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(100.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = povStat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());

        listenerToTest.OnRequestRejected(1);
        BOOST_CHECK_EQUAL(0, nfqStat.getNFQ());
        BOOST_CHECK_CLOSE(0.0, covStat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, covStat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        minMaxVol = povStat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = povStat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
    }

    BOOST_AUTO_TEST_CASE( TestDuplicateReject ) {
        NFQStatistics nfqStat;
        COVStatistics covStat;
        POVStatistics povStat;
        StatisticsProcessor proc(nfqStat, covStat, povStat);
        EventListener listenerToTest(proc);

        listenerToTest.OnInsertOrderRequest(1, 'B', 10.0, 10);
        BOOST_CHECK_EQUAL(0, nfqStat.getNFQ());
        BOOST_CHECK_CLOSE(0.0, covStat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, covStat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        auto minMaxVol = povStat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(100.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = povStat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());

        listenerToTest.OnRequestRejected(1);
        BOOST_CHECK_EQUAL(0, nfqStat.getNFQ());
        BOOST_CHECK_CLOSE(0.0, covStat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, covStat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        minMaxVol = povStat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = povStat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());

        listenerToTest.OnRequestRejected(1);
        BOOST_CHECK_EQUAL(0, nfqStat.getNFQ());
        BOOST_CHECK_CLOSE(0.0, covStat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, covStat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        minMaxVol = povStat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = povStat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
    }

    BOOST_AUTO_TEST_CASE( TestRejectForAccepted ) {
        NFQStatistics nfqStat;
        COVStatistics covStat;
        POVStatistics povStat;
        StatisticsProcessor proc(nfqStat, covStat, povStat);
        EventListener listenerToTest(proc);

        listenerToTest.OnInsertOrderRequest(1, 'B', 10.0, 10);
        BOOST_CHECK_EQUAL(0, nfqStat.getNFQ());
        BOOST_CHECK_CLOSE(0.0, covStat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, covStat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        auto minMaxVol = povStat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(100.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = povStat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());

        listenerToTest.OnRequestAcknowledged(1);
        BOOST_CHECK_EQUAL(0, nfqStat.getNFQ());
        BOOST_CHECK_CLOSE(100.0, covStat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, covStat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        minMaxVol = povStat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(100.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(100.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = povStat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());

        listenerToTest.OnRequestRejected(1);
        BOOST_CHECK_EQUAL(0, nfqStat.getNFQ());
        BOOST_CHECK_CLOSE(100.0, covStat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, covStat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        minMaxVol = povStat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(100.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(100.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = povStat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
    }

    BOOST_AUTO_TEST_CASE( TestInsertAccepted ) {
        NFQStatistics nfqStat;
        COVStatistics covStat;
        POVStatistics povStat;
        StatisticsProcessor proc(nfqStat, covStat, povStat);
        EventListener listenerToTest(proc);

        listenerToTest.OnInsertOrderRequest(1, 'B', 10.0, 10);
        BOOST_CHECK_EQUAL(0, nfqStat.getNFQ());
        BOOST_CHECK_CLOSE(0.0, covStat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, covStat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        auto minMaxVol = povStat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(100.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = povStat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());

        listenerToTest.OnRequestAcknowledged(1);
        BOOST_CHECK_EQUAL(0, nfqStat.getNFQ());
        BOOST_CHECK_CLOSE(100.0, covStat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, covStat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        minMaxVol = povStat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(100.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(100.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = povStat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
    }

    BOOST_AUTO_TEST_CASE( TestDuplicateAccepted ) {
        NFQStatistics nfqStat;
        COVStatistics covStat;
        POVStatistics povStat;
        StatisticsProcessor proc(nfqStat, covStat, povStat);
        EventListener listenerToTest(proc);

        listenerToTest.OnInsertOrderRequest(1, 'B', 10.0, 10);
        BOOST_CHECK_EQUAL(0, nfqStat.getNFQ());
        BOOST_CHECK_CLOSE(0.0, covStat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, covStat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        auto minMaxVol = povStat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(100.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = povStat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());

        listenerToTest.OnRequestAcknowledged(1);
        BOOST_CHECK_EQUAL(0, nfqStat.getNFQ());
        BOOST_CHECK_CLOSE(100.0, covStat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, covStat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        minMaxVol = povStat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(100.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(100.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = povStat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());

        listenerToTest.OnRequestAcknowledged(1);
        BOOST_CHECK_EQUAL(0, nfqStat.getNFQ());
        BOOST_CHECK_CLOSE(100.0, covStat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, covStat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        minMaxVol = povStat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(100.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(100.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = povStat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
    }


    BOOST_AUTO_TEST_CASE( TestInsertReplaced ) {
        NFQStatistics nfqStat;
        COVStatistics covStat;
        POVStatistics povStat;
        StatisticsProcessor proc(nfqStat, covStat, povStat);
        EventListener listenerToTest(proc);

        listenerToTest.OnInsertOrderRequest(1, 'B', 10.0, 10);
        BOOST_CHECK_EQUAL(0, nfqStat.getNFQ());
        BOOST_CHECK_CLOSE(0.0, covStat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, covStat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        auto minMaxVol = povStat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(100.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = povStat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());

        listenerToTest.OnReplaceOrderRequest(1, 2, 20);
        BOOST_CHECK_EQUAL(0, nfqStat.getNFQ());
        BOOST_CHECK_CLOSE(0.0, covStat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, covStat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        minMaxVol = povStat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(300.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = povStat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
    }

    BOOST_AUTO_TEST_CASE( TestDuplicateReplaced ) {
        NFQStatistics nfqStat;
        COVStatistics covStat;
        POVStatistics povStat;
        StatisticsProcessor proc(nfqStat, covStat, povStat);
        EventListener listenerToTest(proc);

        listenerToTest.OnInsertOrderRequest(1, 'B', 10.0, 10);
        BOOST_CHECK_EQUAL(0, nfqStat.getNFQ());
        BOOST_CHECK_CLOSE(0.0, covStat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, covStat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        auto minMaxVol = povStat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(100.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = povStat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());

        listenerToTest.OnReplaceOrderRequest(1, 2, 20);
        BOOST_CHECK_EQUAL(0, nfqStat.getNFQ());
        BOOST_CHECK_CLOSE(0.0, covStat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, covStat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        minMaxVol = povStat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(300.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = povStat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());

        /// duplicate order is ignored
        listenerToTest.OnReplaceOrderRequest(1, 2, 20);
        BOOST_CHECK_EQUAL(0, nfqStat.getNFQ());
        BOOST_CHECK_CLOSE(0.0, covStat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, covStat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        minMaxVol = povStat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(300.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = povStat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
    }

    BOOST_AUTO_TEST_CASE( TestReplaceReplaced ) {
        NFQStatistics nfqStat;
        COVStatistics covStat;
        POVStatistics povStat;
        StatisticsProcessor proc(nfqStat, covStat, povStat);
        EventListener listenerToTest(proc);

        listenerToTest.OnInsertOrderRequest(1, 'B', 10.0, 10);
        BOOST_CHECK_EQUAL(0, nfqStat.getNFQ());
        BOOST_CHECK_CLOSE(0.0, covStat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, covStat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        auto minMaxVol = povStat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(100.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = povStat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());

        listenerToTest.OnReplaceOrderRequest(1, 2, 20);
        BOOST_CHECK_EQUAL(0, nfqStat.getNFQ());
        BOOST_CHECK_CLOSE(0.0, covStat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, covStat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        minMaxVol = povStat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(300.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = povStat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());

        listenerToTest.OnReplaceOrderRequest(2, 3, -10);
        BOOST_CHECK_EQUAL(0, nfqStat.getNFQ());
        BOOST_CHECK_CLOSE(0.0, covStat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, covStat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        minMaxVol = povStat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(300.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = povStat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());

    }

    BOOST_AUTO_TEST_CASE( TestReplaceReplaced_RejectReplaced ) {
        NFQStatistics nfqStat;
        COVStatistics covStat;
        POVStatistics povStat;
        StatisticsProcessor proc(nfqStat, covStat, povStat);
        EventListener listenerToTest(proc);

        listenerToTest.OnInsertOrderRequest(1, 'B', 10.0, 10);
        BOOST_CHECK_EQUAL(0, nfqStat.getNFQ());
        BOOST_CHECK_CLOSE(0.0, covStat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, covStat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        auto minMaxVol = povStat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(100.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = povStat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());

        listenerToTest.OnReplaceOrderRequest(1, 2, 20);
        BOOST_CHECK_EQUAL(0, nfqStat.getNFQ());
        BOOST_CHECK_CLOSE(0.0, covStat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, covStat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        minMaxVol = povStat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(300.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = povStat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());

        listenerToTest.OnReplaceOrderRequest(2, 3, -10);
        BOOST_CHECK_EQUAL(0, nfqStat.getNFQ());
        BOOST_CHECK_CLOSE(0.0, covStat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, covStat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        minMaxVol = povStat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(300.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = povStat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());

        listenerToTest.OnRequestRejected(2);
        BOOST_CHECK_EQUAL(0, nfqStat.getNFQ());
        BOOST_CHECK_CLOSE(0.0, covStat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, covStat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        minMaxVol = povStat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(200.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = povStat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());

    }

    BOOST_AUTO_TEST_CASE( TestReplaceReplaced_Fill ) {
        NFQStatistics nfqStat;
        COVStatistics covStat;
        POVStatistics povStat;
        StatisticsProcessor proc(nfqStat, covStat, povStat);
        EventListener listenerToTest(proc);

        listenerToTest.OnInsertOrderRequest(1, 'B', 10.0, 10);
        BOOST_CHECK_EQUAL(0, nfqStat.getNFQ());
        BOOST_CHECK_CLOSE(0.0, covStat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, covStat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        auto minMaxVol = povStat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(100.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = povStat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());

        listenerToTest.OnReplaceOrderRequest(1, 2, 20);
        BOOST_CHECK_EQUAL(0, nfqStat.getNFQ());
        BOOST_CHECK_CLOSE(0.0, covStat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, covStat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        minMaxVol = povStat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(300.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = povStat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());

        listenerToTest.OnReplaceOrderRequest(2, 3, -10);
        BOOST_CHECK_EQUAL(0, nfqStat.getNFQ());
        BOOST_CHECK_CLOSE(0.0, covStat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, covStat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        minMaxVol = povStat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(300.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = povStat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());

        listenerToTest.OnOrderFilled(1, 5);
        BOOST_CHECK_EQUAL(5, nfqStat.getNFQ());
        BOOST_CHECK_CLOSE(50.0, covStat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, covStat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        minMaxVol = povStat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(50.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(250.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = povStat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());

    }

    BOOST_AUTO_TEST_CASE( TestFillAfterReplaced ) {
        NFQStatistics nfqStat;
        COVStatistics covStat;
        POVStatistics povStat;
        StatisticsProcessor proc(nfqStat, covStat, povStat);
        EventListener listenerToTest(proc);

        listenerToTest.OnInsertOrderRequest(1, 'B', 10.0, 10);
        BOOST_CHECK_EQUAL(0, nfqStat.getNFQ());
        BOOST_CHECK_CLOSE(0.0, covStat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, covStat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        auto minMaxVol = povStat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(100.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = povStat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());

        listenerToTest.OnReplaceOrderRequest(1, 2, 20);
        BOOST_CHECK_EQUAL(0, nfqStat.getNFQ());
        BOOST_CHECK_CLOSE(0.0, covStat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, covStat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        minMaxVol = povStat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(300.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = povStat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());

        listenerToTest.OnRequestAcknowledged(1);
        listenerToTest.OnRequestAcknowledged(2);
        BOOST_CHECK_EQUAL(0, nfqStat.getNFQ());
        BOOST_CHECK_CLOSE(300.0, covStat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, covStat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        minMaxVol = povStat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(300.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(300.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = povStat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());

        listenerToTest.OnOrderFilled(1, 5);
        BOOST_CHECK_EQUAL(5, nfqStat.getNFQ());
        BOOST_CHECK_CLOSE(250.0, covStat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, covStat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        minMaxVol = povStat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(250.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(250.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = povStat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());

        listenerToTest.OnOrderFilled(1, 3);
        BOOST_CHECK_EQUAL(8, nfqStat.getNFQ());
        BOOST_CHECK_CLOSE(220.0, covStat.getCOV(Side::BID), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, covStat.getCOV(Side::OFFER), std::numeric_limits<double>::epsilon());
        minMaxVol = povStat.getPOV(Side::BID);
        BOOST_CHECK_CLOSE(220.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(220.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());
        minMaxVol = povStat.getPOV(Side::OFFER);
        BOOST_CHECK_CLOSE(0.0, std::get<0>(minMaxVol), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(0.0, std::get<1>(minMaxVol), std::numeric_limits<double>::epsilon());

    }

BOOST_AUTO_TEST_SUITE_END()
