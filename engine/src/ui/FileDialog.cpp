#include "ui/FileDialog.h"

namespace Calyx {

    bool File::PassFilter(const String& filter, const String& searchQuery) const {
        // Check directory filter
        if (filter == CX_FILE_FILTER_DIR && type != FileSystem::file_type::directory)
            return false;

        // Check all other filters
        if (filter != CX_FILE_FILTER_ALL && type != FileSystem::file_type::directory && !name.ends_with(filter))
            return false;

        // Check matches search string
        return StringUtils::IsSearchMatch(searchQuery, name);
    }

    FileSelection::FileSelection(const FileSelectionOptions* options) : m_options(options) {}

    void FileSelection::Clear() {
        m_set.clear();
        m_order.clear();
        m_str = "";
        CheckOK();
    }

    void FileSelection::Add(const String& item) {
        bool present = m_set.contains(item);
        if (present || (!m_options->allowMultipleSelection && !m_set.empty()))
            return;

        m_set.insert(item);
        m_order.push_back(item);
        m_str = StringUtils::Join(m_order, ";");
        CheckOK();
    }

    void FileSelection::Remove(const String& item) {
        bool present = m_set.contains(item);
        if (!present)
            return;

        m_set.erase(item);
        std::erase(m_order, item);
        m_str = StringUtils::Join(m_order, ";");
        CheckOK();
    }

    void FileSelection::Normalize() {
        List<String> selection(m_order.begin(), m_order.end());
        m_set.clear();
        m_order.clear();
        for (const auto& selected: selection) {
            Add(PathUtils::Normalize(selected));
        }
    }

    String FileSelection::First(bool nameOnly) const {
        if (IsEmpty()) {
            return "";
        }

        if (nameOnly) {
            return Path(m_order.front()).filename();
        }

        return m_order.front();
    }

    String FileSelection::Last(bool nameOnly) const {
        if (IsEmpty()) {
            return "";
        }

        if (nameOnly) {
            return Path(m_order.back()).filename();
        }

        return m_order.back();
    }

    void FileSelection::CheckOK() {
        if (IsEmpty()) {
            m_ok = false;
            return;
        }

        // Check selection does not contain more than one item
        if (!m_options->allowMultipleSelection && Size() > 1) {
            m_ok = false;
            return;
        }

        // Check all files exist if required, and check all selected files pass filters
        if (std::any_of(
            m_set.begin(), m_set.end(), [this](const String& selection) {
                auto status = FileSystem::status(selection);
                return m_options->requireExistingFiles && !FileSystem::exists(status) || !std::any_of(
                    m_options->filters.begin(), m_options->filters.end(),
                    [this, &selection, &status](const String& filter) {
                        if (m_options->allowAllSelections)
                            return true;
                        if (m_options->allowDirectorySelections && status.type() == FileSystem::file_type::directory)
                            return true;
                        return selection.ends_with(filter);
                    }
                );
            }
        )) {
            m_ok = false;
            return;
        }

        m_ok = true;
    }

    FileSelection& FileSelection::operator=(const FileSelection& other) {
        if (&other == this)
            return *this;
        m_set = other.m_set;
        m_order = other.m_order;
        m_str = other.m_str;
        m_ok = other.m_ok;
        return *this;
    }

    FileSelectionContext::FileSelectionContext(const FileSelectionOptions& options) {
        m_options = options;
        Reset();
    }

    void FileSelectionContext::SetDialogDirectory(Path dir, bool clearSelection) {
        if (!FileSystem::exists(dir))
            dir = PathUtils::FirstExistingFile(dir);
        if (dir.empty())
            return;
        if (clearSelection)
            m_dialogSelection.Clear();
        if (!FileSystem::is_directory(dir)) {
            m_dialogSelection.Clear();
            m_dialogSelection.Add(dir);
            dir = dir.parent_path();
        }
        m_search.clear();
        m_files.clear();
        m_directory = FileSystem::canonical(dir);
        for (const auto& file: FileSystem::directory_iterator(m_directory)) {
            auto path = PathUtils::Normalize(file.path());
            auto type = file.status().type();
            if (type == FileSystem::file_type::not_found)
                continue;
            File dirFile{
                .type = type,
                .modified = Chrono::file_clock::to_sys(FileSystem::last_write_time(path)),
                .path = path,
                .name = path.filename(),
                .size = -1,
            };
            if (dirFile.type == FileSystem::file_type::regular || dirFile.type == FileSystem::file_type::symlink) {
                dirFile.size = FileSystem::file_size(path);
            }
            m_files.push_back(dirFile);
        }
        SortFiles();
    }

    void FileSelectionContext::SortFiles() {
        m_sortedFiles.clear();
        for (const auto& file: m_files) {
            if (file.PassFilter(m_options.filters[m_currentFilter], m_search)) {
                m_sortedFiles.push_back(&file);
            }
        }

        int column = 0;
        ImGuiSortDirection dir = ImGuiSortDirection_Ascending;
        if (m_sortSpecs != nullptr && m_sortSpecs->SpecsCount > 0) {
            column = m_sortSpecs->Specs[0].ColumnIndex;
            dir = m_sortSpecs->Specs[0].SortDirection;
        }
        std::sort(
            m_sortedFiles.begin(),
            m_sortedFiles.end(),
            [this, column, dir](const File* a, const File* b) {
                int cmp = CompareFiles(*a, *b, column, dir);
                return cmp < 0;
            }
        );
    }

    int FileSelectionContext::CompareFiles(
        const File& first,
        const File& second,
        int column,
        ImGuiSortDirection direction
    ) const {
        int nFirst = first.type == FileSystem::file_type::directory ? 0 : 1;
        int nSecond = second.type == FileSystem::file_type::directory ? 0 : 1;
        if (!m_options.showDirectoriesFirst || nFirst - nSecond == 0) {
            int cmp = 0;
            switch (column) {
                case 0:
                    cmp = StringUtils::Compare(first.name, second.name);
                    break;
                case 1:
                    cmp = first.size - second.size;
                    break;
                case 2:
                    cmp = ChronoUtils::Compare(first.modified, second.modified);
                    break;
                default:
                    break;
            }
            return direction == ImGuiSortDirection_Descending ? -cmp : cmp;
        }
        return nFirst - nSecond;
    }

    void FileSelectionContext::Reset() {
        m_selection.Clear();
        m_dialogSelection.Clear();
        m_search = "";
        m_currentFilter = 0;
        SetDialogDirectory(m_options.defaultDirectory);
    }


}