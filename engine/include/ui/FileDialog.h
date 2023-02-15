#pragma once

#define CX_FILE_FILTER_ALL "*"
#define CX_FILE_FILTER_DIR "/"

#include <imgui.h>

namespace Calyx {

    class Widgets;
    class FileSelection;
    class FileSelectionContext;

    struct File {
        FileSystem::file_type type;
        Chrono::time_point<Chrono::system_clock> modified;
        Path path;
        String name;
        int64 size;

        bool PassFilter(const String& filter, const String& searchQuery) const;
    };

    struct FileSelectionOptions {
        List<String> filters = { "*" };
        Path defaultDirectory = PathUtils::GetUserHomeDirectory();
        bool allowMultipleSelection = false;
        bool showDirectoriesFirst = true;
        bool requireExistingFiles = true;

        // Computed properties from filter list
        bool allowAllSelections = std::find(filters.begin(), filters.end(), CX_FILE_FILTER_ALL) != filters.end();
        bool allowDirectorySelections = std::find_if(
            filters.begin(), filters.end(),
            [](const String& filter) {
                return filter == CX_FILE_FILTER_ALL || filter == CX_FILE_FILTER_DIR;
            }
        ) != filters.end();
    };

    class FileSelection {

    public:
        explicit FileSelection(const FileSelectionOptions* options);

        void Clear();
        void Add(const String& item);
        void Remove(const String& item);

        template<typename Container>
        void AddRange(const Container& items) {
            for (const auto& item: items) {
                Add(item);
            }
        }

        void CheckOK();
        void Normalize();

        bool IsOK() const { return m_ok; }
        bool IsEmpty() const { return m_set.empty(); }
        size_t Size() const { return m_set.size(); }
        bool Contains(const String& item) const { return m_set.contains(item); }

        String First(bool nameOnly = false) const;
        String Last(bool nameOnly = false) const;

        auto begin() const { return m_order.begin(); }
        auto end() const { return m_order.end(); }

        FileSelection& operator=(const FileSelection& other);

    private:
        const FileSelectionOptions* m_options;
        Set<String> m_set{};
        Deque<String> m_order{};
        String m_str{};
        bool m_ok = false;

    };

    class FileSelectionContext {
        friend class Widgets;

    public:
        FileSelectionContext() = default;
        explicit FileSelectionContext(const FileSelectionOptions& options);

        void SetDialogDirectory(Path dir, bool clearSelection = true);
        void SortFiles();
        void Reset();

        const FileSelection& GetSelection() const { return m_selection; }

    private:
        int CompareFiles(const File& first, const File& second, int column, ImGuiSortDirection direction) const;

    private:
        FileSelectionOptions m_options;
        FileSelection m_selection{ &m_options };
        FileSelection m_dialogSelection{ &m_options };

        String m_search;
        Path m_directory;

        bool m_showCrumbsInput = false;
        bool m_focusCrumbsInput = false;
        String m_breadCrumbsInput;

        List<File> m_files;
        ImGuiTableSortSpecs* m_sortSpecs = nullptr;
        List<const File*> m_sortedFiles;

        int m_currentFilter = 0;
    };

}