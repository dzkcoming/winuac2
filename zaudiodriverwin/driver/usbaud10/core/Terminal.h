/*
   This file is part of the EMU CA0189 USB Audio Driver.

   Copyright (C) 2008 EMU Systems/Creative Technology Ltd. 

   This driver is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This driver is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library.   If not, a copy of the GNU Lesser General Public 
   License can be found at <http://www.gnu.org/licenses/>.
*/
/*
 *****************************************************************************
 *//*!
 * @file	   Terminal.h
 * @brief	   Terminal definitions.
 * @copyright  E-MU Systems, 2004.
 * @author     hyhuang\@atc.creative.com.
 * @changelog  03-07-2005 1.00 Created.\n
 *//*
 *****************************************************************************
 */
#ifndef __TERMINAL_H__
#define __TERMINAL_H__

#include "Common.h"
#include "UsbDev.h"
#include "usbaudio.h"
#include "Entity.h"

/*****************************************************************************
 * TERMINAL_PARAMETER_BLOCK
 */
typedef struct
{
	struct 
	{
		BOOL	Support;
		ULONG	Current;
	}						CopyProtect;
} TERMINAL_PARAMETER_BLOCK, *PTERMINAL_PARAMETER_BLOCK;

/*****************************************************************************
 * Classes
 */
class CAudioTopology;

/*****************************************************************************
 *//*! @class CTerminal
 *****************************************************************************
 * @ingroup TOPOLOGY_GROUP
 * @brief
 * Topology terminal object.
 */
class CTerminal
:	public CEntity
{
private:

protected:
	PUSB_AUDIO_COMMON_TERMINAL_DESCRIPTOR	m_TerminalDescriptor;

	TERMINAL_PARAMETER_BLOCK				m_ParameterBlock;
	UCHAR									m_ParameterBlockStatusType;

	DEVICE_POWER_STATE						m_PowerState;

	NTSTATUS _RestoreParameterBlock
	(
		IN		UCHAR						ControlSelector,
		IN		BOOL						Support,
		IN		PTERMINAL_PARAMETER_BLOCK	ParameterBlock,
		IN		BOOL						Read
	);

public:
    /*************************************************************************
     * Constructor/destructor.
     */
    /*! @brief Constructor. */
	CTerminal() {}
    /*! @brief Destructor. */
	~CTerminal() {}

    /*************************************************************************
     * CTerminal public methods
     *
     * These are public member functions.  See TOPOLOGY.CPP for specific
	 * descriptions.
     */
	UCHAR TerminalID
	(	void
	);

	USHORT TerminalType
	(	void
	);

	virtual UCHAR iTerminal
	(	void
	) = 0;

	virtual BOOL ParseSources
	(
		IN		UCHAR	Index,
		OUT		UCHAR *	OutSourceID
	) = 0;

	virtual BOOL FindAudioChannelCluster
	(
		OUT		USB_AUDIO_CHANNEL_CLUSTER_DESCRIPTOR *	OutClusterDescriptor
	) = 0;

	ULONG NumberOfChannels
	(	void
	);

	NTSTATUS PowerStateChange
	(
		IN		DEVICE_POWER_STATE	NewState
	);

	NTSTATUS WriteParameterBlock
	(
		IN		UCHAR	RequestCode,
		IN		UCHAR	ControlSelector,
		IN		UCHAR,
		IN		PVOID	ParameterBlock,
		IN		ULONG	ParameterBlockSize,
		IN 		ULONG	Flags = PARAMETER_BLOCK_FLAGS_IO_BOTH
	);

	NTSTATUS ReadParameterBlock
	(
		IN		UCHAR	RequestCode,
		IN		UCHAR	ControlSelector,
		IN		UCHAR,
		IN		PVOID	ParameterBlock,
		IN 		ULONG 	ParameterBlockSize,
		OUT		ULONG *	OutParameterBlockSize,
		IN 		ULONG	Flags = PARAMETER_BLOCK_FLAGS_IO_SOFTWARE
	);

	NTSTATUS RestoreParameterBlock
	(
		IN		PVOID	ParameterBlock = NULL,
		IN		ULONG	ParameterBlockSize = 0
	);

	NTSTATUS SaveParameterBlock
	(
		IN		PVOID	ParameterBlock,
		IN		ULONG	ParameterBlockSize,
		OUT		ULONG *	OutParameterBlockSize
	);

	ULONG GetParameterBlockSize
	(	void
	);

	NTSTATUS UpdateParameterBlock
	(	void
	);

	NTSTATUS InvalidateParameterBlock
	(
		IN		UCHAR	StatusType
	);

	/*************************************************************************
     * Static
     */

    /*************************************************************************
     * Friends
     */
	friend class CList<CTerminal>;
};

typedef CTerminal * PTERMINAL;

/*****************************************************************************
 *//*! @class CInputTerminal
 *****************************************************************************
 * @ingroup TOPOLOGY_GROUP
 * @brief
 * Topology input terminal object.
 */
class CInputTerminal
:	public CTerminal
{
private:
	CAudioTopology *	m_AudioTopology;

	PUSB_AUDIO_INPUT_TERMINAL_DESCRIPTOR	m_InputTerminalDescriptor;

public:
    /*************************************************************************
     * Constructor/destructor.
     */
    /*! @brief Constructor. */
	CInputTerminal() : CTerminal() {}
    /*! @brief Destructor. */
    ~CInputTerminal();
    /*! @brief Self-destructor. */
	void Destruct() { delete this; }

    /*************************************************************************
     * CInputTerminal public methods
     *
     * These are public member functions.  See TOPOLOGY.CPP for specific
	 * descriptions.
     */
	NTSTATUS Init
	(
		IN		CAudioTopology *						AudioTopology,
		IN		PUSB_DEVICE								UsbDevice,
		IN		UCHAR									InterfaceNumber,
		IN		PUSB_AUDIO_COMMON_TERMINAL_DESCRIPTOR	TerminalDescriptor
	);

	UCHAR iTerminal
	(	void
	);

	BOOL ParseSources
	(
		IN		UCHAR	Index,
		OUT		UCHAR *	OutSourceID
	);

	BOOL FindAudioChannelCluster
	(
		OUT		USB_AUDIO_CHANNEL_CLUSTER_DESCRIPTOR *	OutClusterDescriptor
	);

	/*************************************************************************
     * Static
     */

    /*************************************************************************
     * Friends
     */
};

typedef CInputTerminal * PINPUT_TERMINAL;

/*****************************************************************************
 *//*! @class COutputTerminal
 *****************************************************************************
 * @ingroup TOPOLOGY_GROUP
 * @brief
 * Topology output terminal object.
 */
class COutputTerminal
:	public CTerminal
{
private:
	CAudioTopology *	m_AudioTopology;

	PUSB_AUDIO_OUTPUT_TERMINAL_DESCRIPTOR	m_OutputTerminalDescriptor;

public:
    /*************************************************************************
     * Constructor/destructor.
     */
    /*! @brief Constructor. */
	COutputTerminal() : CTerminal() {}
    /*! @brief Destructor. */
    ~COutputTerminal();
    /*! @brief Self-destructor. */
	void Destruct() { delete this; }

    /*************************************************************************
     * COutputTerminal public methods
     *
     * These are public member functions.  See TOPOLOGY.CPP for specific
	 * descriptions.
     */
	NTSTATUS Init
	(
		IN		CAudioTopology *						AudioTopology,
		IN		PUSB_DEVICE								UsbDevice,
		IN		UCHAR									InterfaceNumber,
		IN		PUSB_AUDIO_COMMON_TERMINAL_DESCRIPTOR	TerminalDescriptor
	);

	UCHAR iTerminal
	(	void
	);

	BOOL ParseSources
	(
		IN		UCHAR	Index,
		OUT		UCHAR *	OutSourceID
	);

	BOOL FindAudioChannelCluster
	(
		OUT		USB_AUDIO_CHANNEL_CLUSTER_DESCRIPTOR *	OutClusterDescriptor
	);

	/*************************************************************************
     * Static
     */

    /*************************************************************************
     * Friends
     */
};

typedef COutputTerminal * POUTPUT_TERMINAL;

#endif // __TERMINAL_H__
