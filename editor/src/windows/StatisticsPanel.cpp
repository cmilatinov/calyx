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

        if (Time::Timer("FPS") >= 1.0f) {
            m_fps = m_fpsCounter;
            m_fpsCounter = 0;
            Time::ResetTimer("FPS");
        }

        ImGui::Text("RAM Total: %s", FormatUtils::ByteSize(m_memory.Total).c_str());
        ImGui::Text("RAM Used: %s", FormatUtils::ByteSize(m_memory.Used).c_str());
        ImGui::Text(
            "RAM Process: %s (%s)",
            FormatUtils::ByteSize(m_memory.ProcessUsed).c_str(),
            FormatUtils::Percentage((double)m_memory.ProcessUsed / m_memory.Total).c_str()
        );
        ImGui::Separator();

        ImGui::Text("CPU Usage: %s", FormatUtils::Percentage(m_cpu.ProcessUsage).c_str());
        ImGui::Text("CPU Time: %s", FormatUtils::Milliseconds((float)m_cpuTime / 1000000).c_str());
        ImGui::Text("GPU Time: %s", FormatUtils::Milliseconds((float)m_gpuTime / 1000000).c_str());
        ImGui::Text("FPS: %lu", m_fps);
        ImGui::Separator();

        ImGui::Text("Triangles: %lu", m_primitives);
        ImGui::Text("Indices: %lu", m_primitives * 3);

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
        m_fpsCounter++;
    }

}