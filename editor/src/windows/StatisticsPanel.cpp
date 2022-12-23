#include "windows/StatisticsPanel.h"

namespace Calyx::Editor {

    StatisticsPanel::StatisticsPanel()
        : m_memCollector(100),
          m_cpuCollector(100),
          m_timeElapsedQuery(Query::Create()),
          m_primitivesQuery(Query::Create()) {}

    void StatisticsPanel::Draw() {
        ImGui::Begin("Stats");

        if (Time::Timer("Metrics") >= 0.05f) {
            Utils::Metrics::CollectCPUMetrics(&m_cpu);
            Utils::Metrics::CollectMemoryMetrics(&m_memory);
            m_memCollector.Record(m_memory.ProcessUsed);
            m_cpuCollector.Record(m_cpu.ProcessUsage);
            Time::ResetTimer("Metrics");
        }

        Utils::Graph::Line("CPU Usage (%%)", m_cpuCollector);
        ImGui::Separator();

        ImGui::Text("RAM Total: %s", Utils::Format::ByteSize(m_memory.Total).c_str());
        ImGui::Text("RAM Used: %s",Utils::Format::ByteSize(m_memory.Used).c_str());
        ImGui::Text(
            "RAM Process: %s (%s)",
            Utils::Format::ByteSize(m_memory.ProcessUsed).c_str(),
            Utils::Format::Percentage((double)m_memory.ProcessUsed / m_memory.Total).c_str()
        );
        ImGui::Separator();

        ImGui::Text("CPU Usage: %s", Utils::Format::Percentage(m_cpu.ProcessUsage).c_str());
        ImGui::Text("CPU Time: %s", Utils::Format::Milliseconds((float)m_cpuTime / 1000000).c_str());
        ImGui::Text("GPU Time: %s", Utils::Format::Milliseconds((float)m_gpuTime / 1000000).c_str());
        ImGui::Separator();

        ImGui::Text("Triangles: %llu", m_primitives);
        ImGui::Text("Indices: %llu", m_primitives * 3);

        ImGui::End();
    }

    void StatisticsPanel::BeginFrame() {
        m_lastCPUTime = Clock::now();
        m_primitivesQuery->Begin(QueryType::PRIMITIVES_GENERATED);
        m_timeElapsedQuery->Begin(QueryType::TIME_ELAPSED);
    }

    void StatisticsPanel::EndFrame() {
        m_primitivesQuery->End();
        m_timeElapsedQuery->End();
        m_primitives = m_primitivesQuery->GetValueU64();
        m_gpuTime = m_timeElapsedQuery->GetValueU64();
        m_cpuTime = Chrono::duration_cast<Chrono::Nanoseconds>(Clock::now() - m_lastCPUTime).count();
    }

}