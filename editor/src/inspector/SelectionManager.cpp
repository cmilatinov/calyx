#include "inspector/SelectionManager.h"

namespace Calyx::Editor {

    CX_SINGLETON_INSTANCE(SelectionManager);

    Selection::Iterator::Iterator(const Selection* selection)
        : m_selection(selection), m_iterator(selection->m_selectedItems.cbegin()) {}

    Selection::Iterator::Iterator(const Selection* selection, Set<void*>::const_iterator&& iterator)
        : m_selection(selection), m_iterator(iterator) {}

    Selection::Iterator::value_type Selection::Iterator::operator*() const {
        void* elem = *m_iterator;
        return m_selection->m_type.from_void(elem);
    }

    Selection::Iterator Selection::Iterator::operator++() {
        m_iterator++;
        return *this;
    }

    Selection::Iterator Selection::Iterator::operator++(int) {
        Iterator tmp = *this;
        ++(*this);
        return tmp;
    }

    bool operator==(const Selection::Iterator& left, const Selection::Iterator& right) {
        return left.m_iterator == right.m_iterator;
    }

    bool operator!=(const Selection::Iterator& left, const Selection::Iterator& right) {
        return left.m_iterator != right.m_iterator;
    }

    Selection::Selection(entt::meta_type&& type)
        : m_type(type) {}

}