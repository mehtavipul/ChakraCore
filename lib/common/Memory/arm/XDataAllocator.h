//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#if !defined(_M_ARM)
CompileAssert(false)
#endif

struct XDataAllocation sealed  : public SecondaryAllocation
{
    // ---- Methods ----- //
    XDataAllocation() :
        pdataCount(0)
        , functionTable(NULL)
        , xdataSize(0)
    {}

    RUNTIME_FUNCTION* GetPdataArray() const
    {
        return reinterpret_cast<RUNTIME_FUNCTION*>(address + xdataSize);
    }

    bool IsFreed() const
    {
        return address == nullptr;
    }

    void Free()
    {
        address = nullptr;
        pdataCount = 0;
        functionTable = nullptr;
        xdataSize = 0;
    }

    // ---- Data members ---- //
    ushort pdataCount;                   // ARM requires more than 1 pdata/function
    FunctionTableHandle functionTable;   // stores the handle to the growable function table
    ushort xdataSize;
};

//
// Allocates xdata and pdata entries for ARM architecture on the heap. They are freed when released.
//
//
class XDataAllocator sealed : public SecondaryAllocator
{
// -------- Private members ---------/
private:
    ushort pdataCount;
    FunctionTableHandle* functionTableHandles;

// --------- Public functions ---------/
public:
    XDataAllocator(BYTE* address, uint size);

    bool Initialize(void* segmentStart, void* segmentEnd);
    void Delete();
    bool Alloc(ULONG_PTR functionStart, DWORD functionSize, ushort pdataCount, ushort xdataSize, SecondaryAllocation* allocation);
    void Register(XDataAllocation& allocation, DWORD functionStart, DWORD functionSize);
    void Release(const SecondaryAllocation& address);
    bool CanAllocate();
    DWORD GetAllocSize(ushort pdataCount, ushort xdataSize)
    {
        return sizeof(RUNTIME_FUNCTION) * pdataCount + xdataSize;
    }
};
