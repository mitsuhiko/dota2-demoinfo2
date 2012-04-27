Note Armin: This is the same thing as was released by Valve just that
windows specific stuff was changed so that it compiles on OS X (and
hopefully Linux).


-------------------------------------------------------------------------

Original Readme:

Demoinfo2.exe is a tool that will parse Dota 2 demo files (ending in .dem)
and dump out every message in the demo.  Using this tool, third parties
can parse the demo for various game events to generate information and
statistics.

You can use demoinfo2.exe by itself and parse the demo.  We have also
included the source code to the program itself to show how to parse and
retrieve information directly.

Dota 2's demo format is built around using Google's Protocol Buffers
(protobuf).  Protobuf is a message/object serialization language that
generates code to serialize the objects efficiently.  If you wish to
understand more about protobuf, check out
http://code.google.com/p/protobuf/ 

In order to build demoinfo2 yourself, you will need Visual Studio 2010.
You will also need to download Google's protobuf and snappy libraries.

- Extract demoinfo2.zip and it will make a new folder named "demoinfo2". 

- Download protobuf from
http://code.google.com/p/protobuf/downloads/detail?name=protobuf-2.4.1.zip
Install this directly in the protobuf-2.4.1 directory that was created
when you extracted demoinfo2.

- Open protobuf-2.4.1\vsprojects\protobuf.sln into Visual Studio 2010.
You will be asked to convert it, please do so.  Once loaded, select
Release version from the build target in the toolbar.  Build the
libprotobuf project by right clicking it in the solution list and hitting
"build."  You do not need to build the other protobuf projects.  

- Download protoc (the protobuf compiler) from
http://code.google.com/p/protobuf/downloads/detail?name=protoc-2.4.1-win32.zip
and extract this into the protoc-2.4.1-win32 folder that was created when
you extracted demoinfo2.  This is the protobuf compiler that parses .proto
files and generates C++ code to parse the messages in the demo file.

- The last library needed is Google's snappy compression library.
Download it from
http://code.google.com/p/snappy/downloads/detail?name=snappy-1.0.5.tar.gz
and extract it into the snappy-1.0.5 folder.

Once those libraries are installed, you can now open demoinfo2.sln and
build your own demoinfo2.exe binary.  Select the Release build target from
the toolbar and hit build!  You will have a new binary in
demoinfo2\Release\demoinfo2.exe.

To parse a demo, just download one in Dota 2.  They will be in C:\Program
Files (x86)\Steam\steamapps\common\dota 2 beta\dota\replays\*.dem.  To
dump a demo, just run "demoinfo2 xxx.dem" and it will print out all the
messages in the demo.

Google's protobuf can generate parses in Java and Python using the same
.proto files.  People proficient in those languages can use protoc.exe to
generate code that parses the protobuf messages and parse the demo file in
those languages.

We are curious to see what people come up with using this tool and please
do post questions and comments about it on the Replay section of the Dota
    2 Development Forums at http://dev.dota2.com/forumdisplay.php?f=19 

====== Copyright (c) 2012, Valve Corporation, All rights reserved. ========

 Redistribution and use in source and binary forms, with or without 
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.
 Redistributions in binary form must reproduce the above copyright notice, 
 this list of conditions and the following disclaimer in the documentation 
 and/or other materials provided with the distribution.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE 
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF 
 THE POSSIBILITY OF SUCH DAMAGE.
===========================================================================

