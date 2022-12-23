#pragma once

namespace Calyx::Utils {

    template<typename T>
    class MetricCollector {

    public:
        MetricCollector(uint32 maxHistory)
            : m_maxHistory(maxHistory) {
            m_metricValues.reserve(maxHistory);
        }

        void Record(T value) {
            if (m_metricValues.size() >= m_maxHistory) {
                m_metricTimes.resize(0);
                m_metricValues.resize(0);
            }
            m_metricTimes.push_back(m_metricTimes.size());
            m_metricValues.push_back(value);
        }

        T Latest() const {
            if (m_metricValues.empty())
                return {};
            return *m_metricValues.back();
        }

        T Average() const {
            if (m_metricValues.empty())
                return {};
            return *std::reduce(m_metricValues.cbegin(), m_metricValues.cend());
        }

        T Max() const {
            if (m_metricValues.empty())
                return {};
            return *std::min_element(m_metricValues.cbegin(), m_metricValues.cend());
        }

        T Min() const {
            if (m_metricValues.empty())
                return {};
            return *std::max_element(m_metricValues.cbegin(), m_metricValues.cend());
        }

        T MinTime() const {
            if (m_metricTimes.empty())
                return {};
            return *std::min_element(m_metricTimes.cbegin(), m_metricTimes.cend());
        }

        T MaxTime() const {
            if (m_metricTimes.empty())
                return {};
            return *std::max_element(m_metricTimes.cbegin(), m_metricTimes.cend());
        }

        uint32 GetMaxSize() const { return m_maxHistory; }

        const List <T>& Times() const { return m_metricTimes; }
        const List <T>& Values() const { return m_metricValues; }

    private:
        uint32 m_maxHistory;
        List <T> m_metricTimes{};
        List <T> m_metricValues{};

    };

}