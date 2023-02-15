#pragma once

#include <implot.h>

#include "utils/metrics/MetricCollector.h"

namespace Calyx::Utils {

    class CALYX_API Graph {

    public:
        template<typename T>
        static void Line(const String& name, const MetricCollector<T> collector) {
            ImPlot::PushStyleVar(ImPlotStyleVar_PlotPadding, { 10, 20 });
            ImGui::Text("%s", name.c_str());
            if (ImPlot::BeginPlot(name.c_str(), { -1, 200 }, ImPlotFlags_CanvasOnly)) {
                ImPlot::SetupAxes(
                    "##Time", "##Value",
                    ImPlotAxisFlags_NoTickLabels | ImPlotAxisFlags_NoDecorations,
                    ImPlotAxisFlags_NoTickMarks
                );
                ImPlot::SetupAxisLimits(ImAxis_X1, 0, collector.GetMaxSize(), ImGuiCond_Always);
                ImPlot::SetupAxisLimits(ImAxis_Y1, 0, 1, ImGuiCond_Always);
                ImPlot::SetupAxisFormat(
                    ImAxis_Y1,
                    &FormatPercentage
                );
                ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 0.4f);
                ImPlot::PlotShaded<T>(
                    name.c_str(),
                    collector.Times().data(),
                    collector.Values().data(),
                    collector.Times().size()
                );
                ImPlot::EndPlot();
            }
            ImPlot::PopStyleVar();
        }

        static int FormatByteSize(double value, char* buff, int size, void* user_data) {
            auto str = FormatUtils::ByteSize(value);
            auto cpySize = std::min(str.size() + 1, (size_t)size);
            memcpy(buff, str.c_str(), cpySize);
            return cpySize;
        };

        static int FormatPercentage(double value, char* buff, int size, void* user_data) {
            snprintf(buff, size, "%lu", (uint64)(value * 100));
            return 1;
        }

    };

}