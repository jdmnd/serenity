/*
 * Copyright (c) 2021, Liav A. <liavalb@hotmail.co.il>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <AK/Memory.h>
#include <AK/StdLibExtras.h>
#include <Kernel/Arch/PC/BIOS.h>
#include <Kernel/Devices/MemoryDevice.h>
#include <Kernel/Memory/AnonymousVMObject.h>
#include <Kernel/Sections.h>

namespace Kernel {

UNMAP_AFTER_INIT NonnullRefPtr<MemoryDevice> MemoryDevice::must_create()
{
    return adopt_ref(*new MemoryDevice);
}

UNMAP_AFTER_INIT MemoryDevice::MemoryDevice()
    : CharacterDevice(1, 1)
{
}

UNMAP_AFTER_INIT MemoryDevice::~MemoryDevice()
{
}

KResultOr<size_t> MemoryDevice::read(OpenFileDescription&, u64, UserOrKernelBuffer&, size_t)
{
    TODO();
}

void MemoryDevice::did_seek(OpenFileDescription&, off_t)
{
    TODO();
}

KResultOr<Memory::Region*> MemoryDevice::mmap(Process& process, OpenFileDescription&, Memory::VirtualRange const& range, u64 offset, int prot, bool shared)
{
    auto viewed_address = PhysicalAddress(offset);

    dbgln("MemoryDevice: Trying to mmap physical memory at {} for range of {} bytes", viewed_address, range.size());
    if (!MM.is_allowed_to_mmap_to_userspace(viewed_address, range)) {
        dbgln("MemoryDevice: Trying to mmap physical memory at {} for range of {} bytes failed due to violation of access", viewed_address, range.size());
        return EINVAL;
    }

    auto vmobject = TRY(Memory::AnonymousVMObject::try_create_for_physical_range(viewed_address, range.size()));

    dbgln("MemoryDevice: Mapped physical memory at {} for range of {} bytes", viewed_address, range.size());
    return process.address_space().allocate_region_with_vmobject(
        range,
        move(vmobject),
        0,
        "Mapped Physical Memory",
        prot,
        shared);
}

}
