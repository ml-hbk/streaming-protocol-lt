#include "streaming_protocol/BaseSynchronousSignal.hpp"
#include "streaming_protocol/Unit.hpp"

daq::streaming_protocol::BaseSynchronousSignal::BaseSynchronousSignal(const std::string &signalId, const std::string &tableId, iWriter &writer, LogCallback logCb, std::uint64_t valueIndex)
    : BaseValueSignal(signalId, tableId, writer, logCb)
    , m_valueIndex(valueIndex)
{
}

void daq::streaming_protocol::BaseSynchronousSignal::writeSignalMetaInformation() const
{
    nlohmann::json dataSignal;
    dataSignal[METHOD] = META_METHOD_SIGNAL;
    dataSignal[PARAMS][META_TABLEID] = m_tableId;
    if (!m_interpretationObject.is_null()) {
        dataSignal[PARAMS][META_INTERPRETATION] = m_interpretationObject;
    }

    dataSignal[PARAMS][META_DEFINITION] = getMemberInformation();
    dataSignal[PARAMS][META_VALUEINDEX] = m_valueIndex;
    composeRelatedSignals(dataSignal[PARAMS]);
    m_writer.writeMetaInformation(m_signalNumber, dataSignal);
}

nlohmann::json daq::streaming_protocol::BaseSynchronousSignal::createMember(const std::string &dataType) const
{
    nlohmann::json memberInformation;
    memberInformation[META_NAME] = m_valueName;
    memberInformation[META_DATATYPE] = dataType;
    memberInformation[META_RULE] = META_RULETYPE_EXPLICIT;
    m_unit.compose(memberInformation);
    m_range.compose(memberInformation);
    m_postScaling.compose(memberInformation);
    return memberInformation;
}
