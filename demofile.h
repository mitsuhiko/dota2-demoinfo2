//====== Copyright (c) 2012, Valve Corporation, All rights reserved. ========//
//
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions are met:
//
// Redistributions of source code must retain the above copyright notice, this
// list of conditions and the following disclaimer.
// Redistributions in binary form must reproduce the above copyright notice, 
// this list of conditions and the following disclaimer in the documentation 
// and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE 
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF 
// THE POSSIBILITY OF SUCH DAMAGE.
//===========================================================================//

#ifndef DEMOFILE_H
#define DEMOFILE_H

#include "generated_proto/demo.pb.h"

typedef int32_t             int32;
typedef uint32_t            uint32;
typedef uint64_t            uint64;
typedef uint32_t            CRC32_t;

#define PROTODEMO_HEADER_ID				"PBUFDEM"
#define DEMOFILE_FULLPACKETS_VERSION	2

struct protodemoheader_t
{
	char demofilestamp[ 8 ]; // PROTODEMO_HEADER_ID
	int32 fileinfo_offset;
};

#define MAX_PLAYER_NAME_LENGTH	32
#define MAX_CUSTOM_FILES		4	// max 4 files
#define SIGNED_GUID_LEN			32	// Hashed CD Key (32 hex alphabetic chars + 0 terminator )

typedef struct player_info_s
{
	// network xuid
	uint64			xuid;
	// scoreboard information
	char			name[MAX_PLAYER_NAME_LENGTH];
	// local server user ID, unique while server is running
	int				userID;
	// global unique player identifer
	char			guid[SIGNED_GUID_LEN + 1];
	// friends identification number
	uint32			friendsID;
	// friends name
	char			friendsName[MAX_PLAYER_NAME_LENGTH];
	// true, if player is a bot controlled by game.dll
	bool			fakeplayer;
	// true if player is the HLTV proxy
	bool			ishltv;
#if defined( REPLAY_ENABLED )
	// true if player is the Replay proxy
	bool			isreplay;
#endif
	// custom files CRC for this player
	CRC32_t			customFiles[MAX_CUSTOM_FILES];
	// this counter increases each time the server downloaded a new file
	unsigned char	filesDownloaded;
} player_info_t;

class IDemoMessage
{
public:
	virtual	~IDemoMessage() {};

	virtual EDemoCommands	GetType( void ) const = 0;		// returns module specific header tag
	virtual size_t			GetSize() const = 0;
	virtual ::google::protobuf::Message& GetProtoMsg() = 0;
};

template< EDemoCommands msgType, typename PB_OBJECT_TYPE >
class CDemoMessagePB : public IDemoMessage, public PB_OBJECT_TYPE
{
public:
	CDemoMessagePB() {}
	virtual ~CDemoMessagePB() {}

	virtual EDemoCommands GetType() const 				{ return msgType; }
	virtual size_t GetSize() const  					{ return sizeof( *this ); }
	virtual ::google::protobuf::Message& GetProtoMsg() 	{ return *this; }
};

typedef CDemoMessagePB< DEM_FileHeader, CDemoFileHeader >					CDemoFileHeader_t;
typedef CDemoMessagePB< DEM_FileInfo, CDemoFileInfo >						CDemoFileInfo_t;
typedef CDemoMessagePB< DEM_Stop, CDemoStop >								CDemoStop_t;
typedef CDemoMessagePB< DEM_SyncTick, CDemoSyncTick >						CDemoSyncTick_t;
typedef CDemoMessagePB< DEM_SendTables, CDemoSendTables >					CDemoSendTables_t;
typedef CDemoMessagePB< DEM_ClassInfo, CDemoClassInfo >						CDemoClassInfo_t;
typedef CDemoMessagePB< DEM_StringTables, CDemoStringTables >				CDemoStringTables_t;
typedef CDemoMessagePB< DEM_ConsoleCmd, CDemoConsoleCmd >					CDemoConsoleCmd_t;
typedef CDemoMessagePB< DEM_CustomData, CDemoCustomData >					CDemoCustomData_t;
typedef CDemoMessagePB< DEM_CustomDataCallbacks, CDemoCustomDataCallbacks >	CDemoCustomDataCallbacks_t;
typedef CDemoMessagePB< DEM_UserCmd, CDemoUserCmd >							CDemoUserCmd_t;
typedef CDemoMessagePB< DEM_FullPacket, CDemoFullPacket >					CDemoFullPacket_t;
typedef CDemoMessagePB< DEM_Packet, CDemoPacket >							CDemoPacket_t;

//-----------------------------------------------------------------------------
// Demo file 
//-----------------------------------------------------------------------------
class CDemoFile
{
public:
	CDemoFile();
	~CDemoFile();

	bool	Open(const char *name);
	void	Close();
	bool	IsDone();

	EDemoCommands ReadMessageType( int *pTick, bool *pbCompressed );
	bool	ReadMessage( IDemoMessage *pMsg, bool bCompressed, int *pSize = NULL, int *pUncompressedSize = NULL );

private:
	std::string m_szFileName;

	size_t m_fileBufferPos;
	std::string m_fileBuffer;

	std::string m_parseBufferSnappy;
};

uint32 ReadVarInt32( const std::string& buf, size_t& index );

#endif // DEMOFILE_H

