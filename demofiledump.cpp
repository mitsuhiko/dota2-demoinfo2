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

#include <stdarg.h>
#include "demofile.h"
#include "demofiledump.h"

#include "google/protobuf/descriptor.h"
#include "google/protobuf/reflection_ops.h"
#include "google/protobuf/descriptor.pb.h"

#include "generated_proto/usermessages.pb.h"
#include "generated_proto/ai_activity.pb.h"
#include "generated_proto/dota_modifiers.pb.h"
#include "generated_proto/dota_commonmessages.pb.h"
#include "generated_proto/dota_usermessages.pb.h"

void fatal_errorf( const char* fmt, ... )
{
    va_list  vlist;
    char buf[ 1024 ];

    va_start( vlist, fmt);
    vsnprintf( buf, sizeof( buf ), fmt, vlist );
	buf[ sizeof( buf ) - 1 ] = 0;
    va_end( vlist );

    fprintf( stderr, "\nERROR: %s\n", buf );
    exit( -1 );
}

bool CDemoFileDump::Open( const char *filename )
{
	if ( !m_demofile.Open( filename ) )
	{
		fprintf( stderr, "Couldn't open '%s'\n", filename );
		return false;
	}

	return true;
}

void CDemoFileDump::MsgPrintf( const ::google::protobuf::Message& msg, int size, const char *fmt, ... )
{
	va_list vlist;
	const std::string& TypeName = msg.GetTypeName();

	// Print the message type and size
	printf( "---- %s (%d bytes) -----------------\n", TypeName.c_str(), size );

	va_start( vlist, fmt);
	vprintf( fmt, vlist );
	va_end( vlist );
}

template < class T, int msgType >
void PrintUserMessage( CDemoFileDump& Demo, const void *parseBuffer, int BufferSize )
{
	T msg;

	if( msg.ParseFromArray( parseBuffer, BufferSize ) )
	{
		Demo.MsgPrintf( msg, BufferSize, "%s", msg.DebugString().c_str() );
	}
}

void CDemoFileDump::DumpUserMessage( const void *parseBuffer, int BufferSize )
{
	CSVCMsg_UserMessage userMessage;

	if( userMessage.ParseFromArray( parseBuffer, BufferSize ) )
	{
		int Cmd = userMessage.msg_type();
		int SizeUM = userMessage.msg_data().size();
		const void *parseBufferUM = &userMessage.msg_data()[ 0 ];

		switch( Cmd )
		{
#define HANDLE_UserMsg( _x )			case UM_ ## _x: PrintUserMessage< CUserMsg_ ## _x, UM_ ## _x >( *this, parseBufferUM, SizeUM ); break
#define HANDLE_DOTA_UserMsg( _x )		case DOTA_UM_ ## _x: PrintUserMessage< CDOTAUserMsg_ ## _x, DOTA_UM_ ## _x >( *this, parseBufferUM, SizeUM ); break

		default:
			printf( "WARNING. DumpUserMessage(): Unknown user message %d.\n", Cmd );
			break;

		HANDLE_UserMsg( AchievementEvent );            	// 1,
		HANDLE_UserMsg( CloseCaption );                 // 2,
		//$ HANDLE_UserMsg( CloseCaptionDirect );       // 3,
		HANDLE_UserMsg( CurrentTimescale );             // 4,
		HANDLE_UserMsg( DesiredTimescale );             // 5,
		HANDLE_UserMsg( Fade );                         // 6,
		HANDLE_UserMsg( GameTitle );                    // 7,
		HANDLE_UserMsg( Geiger );                       // 8,
		HANDLE_UserMsg( HintText );                     // 9,
		HANDLE_UserMsg( HudMsg );                       // 10,
		HANDLE_UserMsg( HudText );                      // 11,
		HANDLE_UserMsg( KeyHintText );                  // 12,
		HANDLE_UserMsg( MessageText );                  // 13,
		HANDLE_UserMsg( RequestState );                 // 14,
		HANDLE_UserMsg( ResetHUD );                     // 15,
		HANDLE_UserMsg( Rumble );                       // 16,
		HANDLE_UserMsg( SayText );                      // 17,
		HANDLE_UserMsg( SayText2 );                     // 18,
		HANDLE_UserMsg( SayTextChannel );               // 19,
		HANDLE_UserMsg( Shake );                        // 20,
		HANDLE_UserMsg( ShakeDir );                     // 21,
		HANDLE_UserMsg( StatsCrawlMsg );                // 22,
		HANDLE_UserMsg( StatsSkipState );               // 23,
		HANDLE_UserMsg( TextMsg );                      // 24,
		HANDLE_UserMsg( Tilt );                         // 25,
		HANDLE_UserMsg( Train );                        // 26,
		HANDLE_UserMsg( VGUIMenu );                     // 27,
		HANDLE_UserMsg( VoiceMask );                    // 28,
		HANDLE_UserMsg( VoiceSubtitle );                // 29,
		HANDLE_UserMsg( SendAudio );                    // 30,

		//$ HANDLE_DOTA_UserMsg( AddUnitToSelection );  // 64,
		HANDLE_DOTA_UserMsg( AIDebugLine );             // 65,
		HANDLE_DOTA_UserMsg( ChatEvent );               // 66,
		HANDLE_DOTA_UserMsg( CombatHeroPositions );     // 67,
		HANDLE_DOTA_UserMsg( CombatLogData );           // 68,
		//$ HANDLE_DOTA_UserMsg( CombatLogName );       // 69,
		HANDLE_DOTA_UserMsg( CombatLogShowDeath );      // 70,
		HANDLE_DOTA_UserMsg( CreateLinearProjectile );  // 71,
		HANDLE_DOTA_UserMsg( DestroyLinearProjectile ); // 72,
		HANDLE_DOTA_UserMsg( DodgeTrackingProjectiles );// 73,
		HANDLE_DOTA_UserMsg( GlobalLightColor );        // 74,
		HANDLE_DOTA_UserMsg( GlobalLightDirection );    // 75,
		HANDLE_DOTA_UserMsg( InvalidCommand );          // 76,
		HANDLE_DOTA_UserMsg( LocationPing );            // 77,
		HANDLE_DOTA_UserMsg( MapLine );                 // 78,
		HANDLE_DOTA_UserMsg( MiniKillCamInfo );         // 79,
		HANDLE_DOTA_UserMsg( MinimapDebugPoint );       // 80,
		HANDLE_DOTA_UserMsg( MinimapEvent );            // 81,
		HANDLE_DOTA_UserMsg( NevermoreRequiem );        // 82,
		HANDLE_DOTA_UserMsg( OverheadEvent );           // 83,
		HANDLE_DOTA_UserMsg( SetNextAutobuyItem );      // 84,
		HANDLE_DOTA_UserMsg( SharedCooldown );          // 85,
		HANDLE_DOTA_UserMsg( SpectatorPlayerClick );    // 86,
		HANDLE_DOTA_UserMsg( TutorialTipInfo );         // 87,
		HANDLE_DOTA_UserMsg( UnitEvent );               // 88,
		HANDLE_DOTA_UserMsg( ParticleManager );         // 89,
		HANDLE_DOTA_UserMsg( BotChat );                 // 90,
		HANDLE_DOTA_UserMsg( HudError );                // 91,
		HANDLE_DOTA_UserMsg( ItemPurchased );           // 92,
		HANDLE_DOTA_UserMsg( Ping );                    // 93

#undef HANDLE_UserMsg
#undef HANDLE_DOTA_UserMsg
		}
	}
}

template < class T, int msgType >
void PrintNetMessage( CDemoFileDump& Demo, const void *parseBuffer, int BufferSize )
{
	T msg;

	if( msg.ParseFromArray( parseBuffer, BufferSize ) )
	{
		if( msgType == svc_GameEventList )
		{
			Demo.m_GameEventList.CopyFrom( msg );
		}

		Demo.MsgPrintf( msg, BufferSize, "%s", msg.DebugString().c_str() );
	}
}

template <>
void PrintNetMessage< CSVCMsg_UserMessage, svc_UserMessage >( CDemoFileDump& Demo, const void *parseBuffer, int BufferSize )
{
	Demo.DumpUserMessage( parseBuffer, BufferSize );
}

template <>
void PrintNetMessage< CSVCMsg_GameEvent, svc_GameEvent >( CDemoFileDump& Demo, const void *parseBuffer, int BufferSize )
{
	CSVCMsg_GameEvent msg;

	if( msg.ParseFromArray( parseBuffer, BufferSize ) )
	{
		int iDescriptor;

		for( iDescriptor = 0; iDescriptor < Demo.m_GameEventList.descriptors().size(); iDescriptor++ )
		{
			const CSVCMsg_GameEventList::descriptor_t& Descriptor = Demo.m_GameEventList.descriptors( iDescriptor );

			if( Descriptor.eventid() == msg.eventid() )
				break;
		}

		if( iDescriptor == Demo.m_GameEventList.descriptors().size() )
		{
			printf( "%s", msg.DebugString().c_str() );
		}
		else
		{
			int numKeys = msg.keys().size();
			const CSVCMsg_GameEventList::descriptor_t& Descriptor = Demo.m_GameEventList.descriptors( iDescriptor );

			printf( "%s eventid:%d %s\n", Descriptor.name().c_str(), msg.eventid(),
				msg.has_event_name() ? msg.event_name().c_str() : "" );

			for( int i = 0; i < numKeys; i++ )
			{
				const CSVCMsg_GameEventList::key_t& Key = Descriptor.keys( i );
				const CSVCMsg_GameEvent::key_t& KeyValue = msg.keys( i );

				printf(" %s: ", Key.name().c_str() );

				if( KeyValue.has_val_string() )
					printf( "%s ", KeyValue.val_string().c_str() );
				if( KeyValue.has_val_float() )
					printf( "%f ", KeyValue.val_float() );
				if( KeyValue.has_val_long() )
					printf( "%d ", KeyValue.val_long() );
				if( KeyValue.has_val_short() )
					printf( "%d ", KeyValue.val_short() );
				if( KeyValue.has_val_byte() )
					printf( "%d ", KeyValue.val_byte() );
				if( KeyValue.has_val_bool() )
					printf( "%d ", KeyValue.val_bool() );
				if( KeyValue.has_val_uint64() )
					printf( "%lld ", KeyValue.val_uint64() );

				printf( "\n" );
			}
		}
	}
}

static std::string GetNetMsgName( int Cmd )
{
	if( NET_Messages_IsValid( Cmd ) )
	{
		return NET_Messages_Name( ( NET_Messages )Cmd );
	}
	else if( SVC_Messages_IsValid( Cmd ) )
	{
		return SVC_Messages_Name( ( SVC_Messages )Cmd );
	}

	assert( 0 );
	return "NETMSG_???";
}

void CDemoFileDump::DumpDemoPacket( const std::string& buf )
{
	size_t index = 0;

	while( index < buf.size() )
	{
		int Cmd = ReadVarInt32( buf, index );
		uint32 Size = ReadVarInt32( buf, index );

		if( index + Size > buf.size() )
		{
			const std::string& strName = GetNetMsgName( Cmd );

			fatal_errorf( "buf.ReadBytes() failed. Cmd:%d '%s' \n", Cmd, strName.c_str() );
		}

		switch( Cmd )
		{
#define HANDLE_NetMsg( _x )		case net_ ## _x: PrintNetMessage< CNETMsg_ ## _x, net_ ## _x >( *this, &buf[ index ], Size ); break
#define HANDLE_SvcMsg( _x )		case svc_ ## _x: PrintNetMessage< CSVCMsg_ ## _x, svc_ ## _x >( *this, &buf[ index ], Size ); break

		default:
			printf( "WARNING. DumpUserMessage(): Unknown netmessage %d.\n", Cmd );
			break;

		HANDLE_NetMsg( NOP );            	// 0
		HANDLE_NetMsg( Disconnect );        // 1
		HANDLE_NetMsg( File );              // 2
		HANDLE_NetMsg( SplitScreenUser );   // 3
		HANDLE_NetMsg( Tick );              // 4
		HANDLE_NetMsg( StringCmd );         // 5
		HANDLE_NetMsg( SetConVar );         // 6
		HANDLE_NetMsg( SignonState );       // 7
		HANDLE_SvcMsg( ServerInfo );        // 8
		HANDLE_SvcMsg( SendTable );         // 9
		HANDLE_SvcMsg( ClassInfo );         // 10
		HANDLE_SvcMsg( SetPause );          // 11
		HANDLE_SvcMsg( CreateStringTable ); // 12
		HANDLE_SvcMsg( UpdateStringTable ); // 13
		HANDLE_SvcMsg( VoiceInit );         // 14
		HANDLE_SvcMsg( VoiceData );         // 15
		HANDLE_SvcMsg( Print );             // 16
		HANDLE_SvcMsg( Sounds );            // 17
		HANDLE_SvcMsg( SetView );           // 18
		HANDLE_SvcMsg( FixAngle );          // 19
		HANDLE_SvcMsg( CrosshairAngle );    // 20
		HANDLE_SvcMsg( BSPDecal );          // 21
		HANDLE_SvcMsg( SplitScreen );       // 22
		HANDLE_SvcMsg( UserMessage );       // 23
		//$ HANDLE_SvcMsg( EntityMessage ); // 24
		HANDLE_SvcMsg( GameEvent );         // 25
		HANDLE_SvcMsg( PacketEntities );    // 26
		HANDLE_SvcMsg( TempEntities );      // 27
		HANDLE_SvcMsg( Prefetch );          // 28
		HANDLE_SvcMsg( Menu );              // 29
		HANDLE_SvcMsg( GameEventList );     // 30
		HANDLE_SvcMsg( GetCvarValue );      // 31

#undef HANDLE_SvcMsg
#undef HANDLE_NetMsg
		}

		index += Size;
	}
}

static bool DumpDemoStringTable( CDemoFileDump& Demo, const CDemoStringTables& StringTables )
{
	for( int i = 0; i < StringTables.tables().size(); i++ )
	{
		const CDemoStringTables::table_t& Table = StringTables.tables( i );

		printf( "#%d %s flags:0x%x (%d Items) %d bytes\n",
			i, Table.table_name().c_str(), Table.table_flags(),
			Table.items().size() + Table.items_clientside().size(), Table.ByteSize() );

		bool bIsActiveModifiersTable = !strcmp( Table.table_name().c_str(), "ActiveModifiers" );
		bool bIsUserInfo = !strcmp( Table.table_name().c_str(), "userinfo" );

		// Only spew out the stringtables (really big) if verbose is on.
		for( int itemid = 0; itemid < Table.items().size(); itemid++ )
		{
			const CDemoStringTables::items_t& Item = Table.items( itemid );

			if( bIsActiveModifiersTable )
			{
				CDOTAModifierBuffTableEntry Entry;

				if( Entry.ParseFromString( Item.data() ) )
				{
					std::string EntryStr = Entry.DebugString();
					printf( "    #%d %s", itemid, EntryStr.c_str() );
					continue;
				}
			}
			else if( bIsUserInfo && Item.data().size() == sizeof( player_info_s ) )
			{
				const player_info_s *pPlayerInfo = ( const player_info_s * )&Item.data()[ 0 ];

				printf("    xuid:%lld name:%s userID:%d guid:%s friendsID:%d friendsName:%s fakeplayer:%d ishltv:%d filesDownloaded:%d\n",
					pPlayerInfo->xuid, pPlayerInfo->name, pPlayerInfo->userID, pPlayerInfo->guid, pPlayerInfo->friendsID,
					pPlayerInfo->friendsName, pPlayerInfo->fakeplayer, pPlayerInfo->ishltv, pPlayerInfo->filesDownloaded );
			}

			printf( "    #%d '%s' (%d bytes)\n", itemid, Item.str().c_str(), (int)Item.data().size() );
		}

		for( int itemid = 0; itemid < Table.items_clientside().size(); itemid++ )
		{
			const CDemoStringTables::items_t& Item = Table.items_clientside( itemid );

			printf( "    %d. '%s' (%d bytes)\n", itemid, Item.str().c_str(), (int)Item.data().size() );
		}
	}

	return true;
}

void CDemoFileDump::PrintDemoHeader( EDemoCommands DemoCommand, int tick, int size, int uncompressed_size )
{
	const std::string& DemoCommandName = EDemoCommands_Name( DemoCommand );

	printf( "==== #%d: Tick:%d '%s' Size:%d UncompressedSize:%d ====\n",
		m_nFrameNumber, tick, DemoCommandName.c_str(), size, uncompressed_size );
}

template < class DEMCLASS >
void PrintDemoMessage( CDemoFileDump& Demo, bool bCompressed, int tick, int& size, int& uncompressed_size )
{
	DEMCLASS Msg;

	if( Demo.m_demofile.ReadMessage( &Msg, bCompressed, &size, &uncompressed_size ) )
	{
		Demo.PrintDemoHeader( Msg.GetType(), tick, size, uncompressed_size );

		Demo.MsgPrintf( Msg, size, "%s", Msg.DebugString().c_str() );
	}
}

template <>
void PrintDemoMessage<CDemoStringTables_t>( CDemoFileDump& Demo, bool bCompressed, int tick, int& size, int& uncompressed_size )
{
	CDemoStringTables_t Msg;

	if( Demo.m_demofile.ReadMessage( &Msg, bCompressed, &size, &uncompressed_size ) )
	{
		Demo.PrintDemoHeader( Msg.GetType(), tick, size, uncompressed_size );

		DumpDemoStringTable( Demo, Msg );
	}
}

void CDemoFileDump::DoDump()
{
	bool bStopReading = false;

	for( m_nFrameNumber = 0; !bStopReading; m_nFrameNumber++ )
	{
		int tick = 0;
		int size = 0;
		bool bCompressed;
		int uncompressed_size = 0;

		if( m_demofile.IsDone() )
			break;

		EDemoCommands DemoCommand = m_demofile.ReadMessageType( &tick, &bCompressed );

		switch( DemoCommand )
		{
#define HANDLE_DemoMsg( _x )	case DEM_ ## _x: PrintDemoMessage< CDemo ## _x ## _t >( *this, bCompressed, tick, size, uncompressed_size ); break

		HANDLE_DemoMsg( FileHeader );
		HANDLE_DemoMsg( FileInfo );
		HANDLE_DemoMsg( Stop );
		HANDLE_DemoMsg( SyncTick );
		HANDLE_DemoMsg( ConsoleCmd );
		HANDLE_DemoMsg( SendTables );
		HANDLE_DemoMsg( ClassInfo );
		HANDLE_DemoMsg( StringTables );
		HANDLE_DemoMsg( UserCmd );
		HANDLE_DemoMsg( CustomDataCallbacks );
		HANDLE_DemoMsg( CustomData );

#undef HANDLE_DemoMsg

		case DEM_FullPacket:
			{
				CDemoFullPacket_t FullPacket;

				if( m_demofile.ReadMessage( &FullPacket, bCompressed, &size, &uncompressed_size ) )
				{
					PrintDemoHeader( DemoCommand, tick, size, uncompressed_size );

					// Spew the stringtable
					DumpDemoStringTable( *this, FullPacket.string_table() );

					// Ok, now the packet.
					DumpDemoPacket( FullPacket.packet().data() );
				}
			}
			break;

		case DEM_Packet:
		case DEM_SignonPacket:
			{
				CDemoPacket_t Packet;

				if( m_demofile.ReadMessage( &Packet, bCompressed, &size, &uncompressed_size ) )
				{
					PrintDemoHeader( DemoCommand, tick, size, uncompressed_size );

					DumpDemoPacket( Packet.data() );
				}
			}
			break;

		default:
		case DEM_Error:
			bStopReading = true;
			fatal_errorf( "Shouldn't ever get this demo command?!? %d\n", DemoCommand );
			break;
		}
	}
}

