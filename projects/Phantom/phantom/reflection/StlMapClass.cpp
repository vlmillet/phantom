// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

/* ******************* Includes ****************** */
// #include "phantom/phantom.h"
#include "StlMapClass.h"

#include "Method.h"
#include "phantom/new.h"
/* *********************************************** */
namespace phantom
{
namespace reflection
{
StlMapClass::StlMapClass(StringView a_strName, size_t a_uiSize, size_t a_uiAlignment,
                         Modifiers a_Modifiers, uint a_uiFlags)
    : ContainerClass(TypeKind::StlMapClass, a_strName, a_uiSize, a_uiAlignment, a_Modifiers,
                     a_uiFlags)
{
}

StlMapClass::StlMapClass(StringView a_strName, Modifiers a_Modifiers /*= 0*/,
                         uint a_uiFlags /*= 0*/)
    : ContainerClass(TypeKind::StlMapClass, a_strName, a_Modifiers, a_uiFlags),
      m_Data(PHANTOM_NEW(Data))
{
}

StlMapClass::~StlMapClass()
{
    PHANTOM_DELETE(Data) m_Data;
}

void StlMapClass::eraseKey(void* a_pContainer, void const* a_pKey) const
{
    PHANTOM_ASSERT(m_pKeyType && m_pMappedType);
    if (!m_Data->m_pFunc_eraseKey)
        m_Data->m_pFunc_eraseKey = getMethod("erase", {m_pKeyType->addConstLValueReference()});
    void* args[] = {(void*)a_pKey};
    m_Data->m_pFunc_eraseKey->invoke(a_pContainer, args);
}

void StlMapClass::insert(void* a_pContainer, void const* a_pPair) const
{
    PHANTOM_ASSERT(m_pKeyType && m_pMappedType);
    if (!m_Data->m_pFunc_insert)
        m_Data->m_pFunc_insert = getMethod("insert", {getValueType()->addConstLValueReference()});
    void* args[] = {(void*)a_pPair};
    m_Data->m_pFunc_insert->invoke(a_pContainer, args);
}

void StlMapClass::map(void* a_pContainer, void const* a_pKey, void* a_pOutPairPointer) const
{
    PHANTOM_ASSERT(m_pKeyType && m_pMappedType);
    if (!m_Data->m_pFunc_map)
        m_Data->m_pFunc_map = getMethod("operator[]", {m_pKeyType->addConstLValueReference()});
    void* args[] = {(void*)a_pKey};
    m_Data->m_pFunc_map->invoke(a_pContainer, args, a_pOutPairPointer);
}

void StlMapClass::find(void const* a_pContainer, void const* a_pKey, void* a_pOutIt) const
{
    PHANTOM_ASSERT(m_pKeyType && m_pMappedType);
    if (!m_Data->m_pFunc_findc)
        m_Data->m_pFunc_findc =
        getMethod("find", {m_pKeyType->addConstLValueReference()}, Modifier::Const);
    void* args[] = {(void*)a_pKey};
    m_Data->m_pFunc_findc->invoke((void*)a_pContainer, args, a_pOutIt);
}

void StlMapClass::find(void* a_pContainer, void const* a_pKey, void* a_pOutIt) const
{
    PHANTOM_ASSERT(m_pKeyType && m_pMappedType);
    if (!m_Data->m_pFunc_find)
        m_Data->m_pFunc_find =
        getMethod("find", {m_pKeyType->addConstLValueReference()}, Modifier::Const);
    void* args[] = {(void*)a_pKey};
    m_Data->m_pFunc_find->invoke((void*)a_pContainer, args, a_pOutIt);
}

} // namespace reflection
} // namespace phantom
