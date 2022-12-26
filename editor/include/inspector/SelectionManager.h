#pragma once

#include "ecs/GameObject.h"

namespace Calyx::Editor {

    class Selection {

    public:
        struct Iterator {
            friend class Selection;

            using value_type = entt::meta_any;

        private:
            explicit Iterator(const Selection* selection);
            Iterator(const Selection* selection, Set<void*>::const_iterator&& iterator);

        public:
            value_type operator*() const;

            Iterator operator++();
            Iterator operator++(int);

            friend bool operator==(const Iterator& left, const Iterator& right);
            friend bool operator!=(const Iterator& left, const Iterator& right);

        private:
            const Selection* m_selection;
            Set<void*>::const_iterator m_iterator;

        };

    private:
        explicit Selection(entt::meta_type&& type);

    public:
        Selection() = default;

        template<typename T>
        Selection& AddItem(T* item) {
            if (entt::resolve<T>().id() == m_type.id()) {
                m_selectedItems.insert(reinterpret_cast<void*>(item));
            }
            return *this;
        }

        template<typename T>
        Selection& AddItem(const Ref<T>& item) {
            return AddItem(item.get());
        }

        entt::meta_any First() const { return *begin(); }

        size_t Size() const { return m_selectedItems.size(); }
        bool IsInvalid() const { return !m_type; }
        bool IsEmpty() const { return m_selectedItems.empty(); }
        bool IsGameObjectSelection() const { return m_type.id() == entt::resolve<GameObject>().id(); }

        template<typename T>
        bool IsItemSelected(T* item) const { return m_selectedItems.contains(reinterpret_cast<void*>(item)); }

        const entt::meta_type& GetType() const { return m_type; }

        Iterator begin() const { return Iterator(this); }
        Iterator end() const { return Iterator(this, m_selectedItems.cend()); }

        template<typename T>
        static Selection Create() { return Selection(entt::resolve<T>()); }

    private:
        entt::meta_type m_type;
        Set<void*> m_selectedItems{};

    };

    class SelectionManager {
    CX_SINGLETON(SelectionManager);

    public:
        SelectionManager() = default;

        CX_SINGLETON_EXPOSE_METHOD(
            _SetCurrentSelection,
            void SetCurrentSelection(const Selection& selection),
            selection
        );
        CX_SINGLETON_EXPOSE_METHOD(_SetCurrentSelection, void SetCurrentSelection(Selection&& selection), selection);
        CX_SINGLETON_EXPOSE_METHOD(_GetCurrentSelection, const Selection& GetCurrentSelection());

        template<typename T>
        static inline bool IsSelected(T* item) { return s_instance->_IsSelected<T>(item); }

    private:
        Selection m_currentSelection;

        void _SetCurrentSelection(const Selection& selection) { m_currentSelection = selection; }
        void _SetCurrentSelection(Selection&& selection) { m_currentSelection = selection; }
        const Selection& _GetCurrentSelection() const { return m_currentSelection; }

        template<typename T>
        bool _IsSelected(T* item) const { return m_currentSelection.IsItemSelected<T>(item); }

    };

}
