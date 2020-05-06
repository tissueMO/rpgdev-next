//###################################################################
//			グルーコード
//###################################################################
#include "Header.h"

void CScript::SQGlue() {
Sqrat::RootTable().Bind("CNullCheck", Sqrat::Class<CNullCheck>()
.Func("IsNull", (&CNullCheck::IsNull), 0, NULL)			//bool|
);

Sqrat::RootTable().Bind("CIDNameData", Sqrat::DerivedClass<CIDNameData, CNullCheck>()
.Var("ID", &CIDNameData::ID)			//int
.Var("Name", &CIDNameData::Name)			//string
);

Sqrat::RootTable().Bind("CIDValueData", Sqrat::DerivedClass<CIDValueData, CIDNameData>()
.Var("Value", &CIDValueData::Value)			//int
);

Sqrat::RootTable().Bind("CSoundData", Sqrat::Class<CSoundData>()
.Prop("FileName", &CSoundData::getFileName)			//string
.Prop("Volume", &CSoundData::getVolume)			//int
.Prop("Pitch", &CSoundData::getPitch)			//int
.Prop("Panpot", &CSoundData::getPanpot)			//int
.Prop("Handle", &CSoundData::getHandle)			//int
.Func("Load", ((void(CSoundData::*)(string, int, int, int))&CSoundData::Load), 5, ".snnn")			//void|string fileName,int vol,int pit,int pan
.Func("Play", (&CSoundData::Play), 3, ".nb|i")			//void|int playType,bool oncePlay
.Func("Stop", (&CSoundData::Stop), 0, NULL)			//void|
.Func("Pause", (&CSoundData::Pause), 0, NULL)			//void|
.Func("Resume", (&CSoundData::Resume), 2, ".b|i")			//void|bool loopMode
.Func("Dispose", (&CSoundData::Dispose), 0, NULL)			//void|
);

Sqrat::RootTable().Bind("CPoint", Sqrat::DerivedClass<CPoint, CNullCheck>()
.Var("X", &CPoint::X)			//int
.Var("Y", &CPoint::Y)			//int
.Func("Set", ((void(CPoint::*)(int, int))&CPoint::Set), 3, ".nn")			//void|int x,int y
.Func("IsEmpty", (&CPoint::IsEmpty), 0, NULL)			//bool|
);

Sqrat::RootTable().Bind("CSize", Sqrat::DerivedClass<CSize, CNullCheck>()
.Var("Width", &CSize::Width)			//int
.Var("Height", &CSize::Height)			//int
.Func("Set", ((void(CSize::*)(int, int))&CSize::Set), 3, ".nn")			//void|int width,int height
.Func("IsEmpty", (&CSize::IsEmpty), 0, NULL)			//bool|
);

Sqrat::RootTable().Bind("CRectangle", Sqrat::DerivedClass<CRectangle, CNullCheck>()
.Var("Left", &CRectangle::Left)			//int
.Var("Top", &CRectangle::Top)			//int
.Var("Width", &CRectangle::Width)			//int
.Var("Height", &CRectangle::Height)			//int
.Func("Set", ((void(CRectangle::*)(int, int, int, int))&CRectangle::Set), 5, ".nnnn")			//void|int left,int top,int width,int height
.Func("SetByCorner", ((void(CRectangle::*)(int, int, int, int))&CRectangle::SetByCorner), 5, ".nnnn")			//void|int left,int top,int right,int bottom
.Func("Right", (&CRectangle::Right), 0, NULL)			//int|
.Func("Bottom", (&CRectangle::Bottom), 0, NULL)			//int|
.Func("PointContains", ((bool(CRectangle::*)(int, int))&CRectangle::PointContains), 3, ".nn")			//bool|int x,int y
);

Sqrat::RootTable().Bind("CCircle", Sqrat::Class<CCircle>()
.Var("Radius", &CCircle::Radius)			//int
.Var("CenterX", &CCircle::CenterX)			//int
.Var("CenterY", &CCircle::CenterY)			//int
.Func("Set", (&CCircle::Set), 4, ".nnn")			//void|int x,int y,int r
.Func("FromRectangle", (&CCircle::FromRectangle), 2, ".x")			//void|const CRectangle& rect
.Func("ToRectangle", (&CCircle::ToRectangle), 0, NULL)			//CRectangle|
.Func("PointContains", (&CCircle::PointContains), 3, ".nn")			//bool|int x,int y
.Func("CircleContains", (&CCircle::CircleContains), 2, ".x")			//bool|const CCircle& c
);

Sqrat::RootTable().Bind("CRGBColor", Sqrat::DerivedClass<CRGBColor, CNullCheck>()
.Var("Red", &CRGBColor::Red)			//int
.Var("Green", &CRGBColor::Green)			//int
.Var("Blue", &CRGBColor::Blue)			//int
.Func("Set", ((void(CRGBColor::*)(int, int, int))&CRGBColor::Set), 4, ".nnn")			//void|int r,int g,int b
.Func("ToCode", (&CRGBColor::ToCode), 0, NULL)			//int|
);

Sqrat::RootTable().Bind("CIntegerArray", Sqrat::DerivedClass<CIntegerArray, CNullCheck>()
.Func("Add", (&CIntegerArray::Add), 2, ".n")			//void|int value
.Func("Exists", (&CIntegerArray::Exists), 2, ".n")			//bool|int value
.Func("Count", (&CIntegerArray::Count), 0, NULL)			//int|
.Func("Get", (&CIntegerArray::Get), 2, ".n")			//int|int index
.Func("Set", (&CIntegerArray::Set), 3, ".nn")			//void|int index,int value
.Func("Insert", (&CIntegerArray::Insert), 3, ".nn")			//void|int index,int value
.Func("Delete", (&CIntegerArray::Delete), 2, ".n")			//void|int index
.Func("Clear", (&CIntegerArray::Clear), 0, NULL)			//void|
);

Sqrat::RootTable().Bind("CStringArray", Sqrat::DerivedClass<CStringArray, CNullCheck>()
.Func("Add", (&CStringArray::Add), 2, ".s")			//void|string value
.Func("Exists", (&CStringArray::Exists), 2, ".s")			//bool|string value
.Func("Count", (&CStringArray::Count), 0, NULL)			//int|
.Func("Get", (&CStringArray::Get), 2, ".n")			//string|int index
.Func("Set", (&CStringArray::Set), 3, ".ns")			//void|int index,string value
.Func("Insert", (&CStringArray::Insert), 3, ".ns")			//void|int index,string value
.Func("Delete", (&CStringArray::Delete), 2, ".n")			//void|int index
.Func("Clear", (&CStringArray::Clear), 0, NULL)			//void|
);

Sqrat::RootTable().Bind("CSelfValueData", Sqrat::DerivedClass<CSelfValueData, CNullCheck>()
.Func("Get", (&CSelfValueData::Get), 2, ".n")			//CIDValueData*|int ID
.Func("Set", (&CSelfValueData::Set), 3, ".nn")			//void|int ID,int val
);

Sqrat::RootTable().Bind("CIPv4Address", Sqrat::Class<CDXLibWrapper::CIPv4Address>()
.Func("Set", (&CDXLibWrapper::CIPv4Address::Set), 5, ".nnnn")			//void|int d1,int d2,int d3,int d4
.Func("Get", (&CDXLibWrapper::CIPv4Address::Get), 2, ".n")			//int|int d
.Func("ToString", (&CDXLibWrapper::CIPv4Address::ToString), 0, NULL)			//string|
);

Sqrat::RootTable().Bind("CDXLibWrapper", Sqrat::Class<CDXLibWrapper>()
.Prop("Offset", &CDXLibWrapper::getOffset)			//CPoint
.Prop("KeyInputResult", &CDXLibWrapper::getKeyInputResult)			//int
.Prop("RecvData", &CDXLibWrapper::getRecvData)			//string
.Prop("UDPRecvIP", &CDXLibWrapper::getUDPRecvIP)			//CIPv4Address
.Prop("UDPRecvPortNum", &CDXLibWrapper::getUDPRecvPortNum)			//int
.Func("Flash", (&CDXLibWrapper::Flash), 5, ".nnnn")			//void|int color,int strength,int speed,int count
.Func("ShakeOne", (&CDXLibWrapper::ShakeOne), 2, ".n")			//void|int strength
.Func("StartShake", (&CDXLibWrapper::StartShake), 2, ".n")			//void|int strength
.Func("StopShake", (&CDXLibWrapper::StopShake), 0, NULL)			//void|
.Func("DrawWindow", (&CDXLibWrapper::DrawWindow), 6, ".nnnnn")			//void|int hdl,int intX,int intY,int intWidth,int intHeight
.Func("DrawCursorPanel", (&CDXLibWrapper::DrawCursorPanel), 6, ".nnnnn")			//void|int hdl,int intX,int intY,int intWidth,int intHeight
.Func("DrawCursorArrow", (&CDXLibWrapper::DrawCursorArrow), 5, ".nnnn")			//void|int hdl,int intX,int intY,int intDir4
.Func("DrawPixel", (&CDXLibWrapper::DrawPixel), 4, ".nnn")			//void|int x,int y,int color
.Func("DrawLine", (&CDXLibWrapper::DrawLine), 7, ".nnnnnn")			//void|int sx,int sy,int ex,int ey,int color,int thickness = 1
.Func("DrawTriangle", (&CDXLibWrapper::DrawTriangle), 9, ".nnnnnnnb|i")			//void|int ax,int ay,int bx,int by,int cx,int cy,int color,bool fill = true
.Func("DrawBox", (&CDXLibWrapper::DrawBox), 8, ".nnnnnb|in")			//void|int left,int top,int right,int bottom,int color,bool fill = true,int thickness = 1
.Func("DrawRect", (&CDXLibWrapper::DrawRect), 8, ".nnnnnb|in")			//void|int left,int top,int width,int height,int color,bool fill = true,int thickness = 1
.Func("DrawCircle", (&CDXLibWrapper::DrawCircle), 7, ".nnnnb|in")			//void|int x,int y,int r,int color,bool fill = true,int thickness = 1
.Func("DrawOval", (&CDXLibWrapper::DrawOval), 8, ".nnnnnb|in")			//void|int x,int y,int rx,int ry,int color,bool fill = true,int thickness = 1
.Func("DrawGraph", (&CDXLibWrapper::DrawGraph), 6, ".nnnb|ib|i")			//void|int Handle,int x,int y,bool trans = true,bool turn = false
.Func("DrawExtendGraph", (&CDXLibWrapper::DrawExtendGraph), 7, ".nnnnnb|i")			//void|int Handle,int x,int y,double xRate,double yRate = 0.0F,bool trans = true
.Func("DrawRotaGraph", (&CDXLibWrapper::DrawRotaGraph), 8, ".nnnnnb|ib|i")			//void|int Handle,int cx,int cy,double angle,double exRate = 1.0F,bool trans = true,bool turn = false
.Func("DrawRectGraph", (&CDXLibWrapper::DrawRectGraph), 10, ".nnnnnnnb|ib|i")			//void|int Handle,int sx,int sy,int gx,int gy,int width,int height,bool trans = true,bool turn = false
.Func("DrawRectExtendGraph", (&CDXLibWrapper::DrawRectExtendGraph), 11, ".nnnnnnnnnb|i")			//void|int Handle,int left,int top,int width,int height,int gx,int gy,int rawWidth,int rawHeight,bool trans = true
.Func("DrawRectRotaGraph", (&CDXLibWrapper::DrawRectRotaGraph), 12, ".nnnnnnnnnb|ib|i")			//void|int Handle,int cx,int cy,int gx,int gy,int rawWidth,int rawHeight,double angle,double exRate = 1.0F,bool trans = true,bool turn = false
.Func("DrawModifiedGraph", (&CDXLibWrapper::DrawModifiedGraph), 11, ".nnnnnnnnnb|i")			//void|int Handle,int leftTopX,int leftTopY,int rightTopX,int rightTopY,int rightBottomX,int rightBottomY,int leftBottomX,int leftBottomY,bool trans = true
.Func("DrawBlendGraph", (&CDXLibWrapper::DrawBlendGraph), 7, ".nnnb|isn")			//void|int x,int y,int Handle,bool trans,string blendFileName,int borderParam
.Func("DrawString", (&CDXLibWrapper::DrawString), 6, ".nnnns")			//void|int x,int y,int fontID,int color,const TCHAR* text
.Func("DrawStringC", (&CDXLibWrapper::DrawStringC), 6, ".nnnns")			//void|int centerX,int y,int fontID,int color,const TCHAR* text
.Func("DrawStringR", (&CDXLibWrapper::DrawStringR), 6, ".nnnns")			//void|int right,int y,int fontID,int color,const TCHAR* text
.Func("DrawStringAlign", (&CDXLibWrapper::DrawStringAlign), 7, ".nnnnns")			//void|int x,int y,int align,int fontID,int color,const TCHAR* text
.Func("GetGraphSize", (&CDXLibWrapper::GetGraphSize), 2, ".n")			//CSize|int Handle
.Func("SetDrawBlendMode", (&CDXLibWrapper::SetDrawBlendMode), 3, ".nn")			//void|int mode,int param
.Func("RestoreDrawBlendMode", (&CDXLibWrapper::RestoreDrawBlendMode), 0, NULL)			//void|
.Func("SetDrawBright", (&CDXLibWrapper::SetDrawBright), 4, ".nnn")			//void|int r,int g,int b
.Func("ResetDrawArea", (&CDXLibWrapper::ResetDrawArea), 0, NULL)			//void|
.Func("SetDrawArea", ((void(CDXLibWrapper::*)(int, int, int, int))&CDXLibWrapper::SetDrawArea), 5, ".nnnn")			//void|int left,int top,int right,int bottom
.Func("GetDrawStringSize", (&CDXLibWrapper::GetDrawStringSize), 3, ".ns")			//CSize|int fontID,const TCHAR* text
.Func("GetColor", (&CDXLibWrapper::GetColor), 4, ".nnn")			//int|int r,int g,int b
.Func("PlayVideo", (&CDXLibWrapper::PlayVideo), 4, ".snb|i")			//void|string fileName,int exRate,bool canCancel
.Func("PlayMovieToGraph", (&CDXLibWrapper::PlayMovieToGraph), 2, ".n")			//void|int Handle
.Func("PauseMovieToGraph", (&CDXLibWrapper::PauseMovieToGraph), 2, ".n")			//void|int Handle
.Func("SeekMovieToGraph", (&CDXLibWrapper::SeekMovieToGraph), 3, ".nn")			//void|int Handle,int timeMS
.Func("TellMovieToGraph", (&CDXLibWrapper::TellMovieToGraph), 2, ".n")			//int|int Handle
.Func("GetMovieStateToGraph", (&CDXLibWrapper::GetMovieStateToGraph), 2, ".n")			//int|int Handle
.Func("GetJoypadNum", (&CDXLibWrapper::GetJoypadNum), 0, NULL)			//int|
.Func("GetJoypadInputState", (&CDXLibWrapper::GetJoypadInputState), 2, ".n")			//int|int playerNum
.Func("GetJoypadAnalogInput", (&CDXLibWrapper::GetJoypadAnalogInput), 2, ".n")			//CPoint|int playerNum
.Func("StartJoypadVibration", (&CDXLibWrapper::StartJoypadVibration), 4, ".nnn")			//void|int playerNum,int power,int time
.Func("StopJoypadVibration", (&CDXLibWrapper::StopJoypadVibration), 2, ".n")			//void|int playerNum
.Func("SetMouseDispFlag", (&CDXLibWrapper::SetMouseDispFlag), 2, ".b|i")			//void|bool flag
.Func("GetMousePoint", (&CDXLibWrapper::GetMousePoint), 0, NULL)			//CPoint|
.Func("SetMousePoint", (&CDXLibWrapper::SetMousePoint), 3, ".nn")			//void|int x,int y
.Func("GetMouseInput", (&CDXLibWrapper::GetMouseInput), 0, NULL)			//int|
.Func("GetMouseWheelRotVol", (&CDXLibWrapper::GetMouseWheelRotVol), 0, NULL)			//int|
.Func("KeyInputString", (&CDXLibWrapper::KeyInputString), 5, ".nnnb|i")			//string|int x,int y,int maxCharLength,bool allowCancel
.Func("KeyInputSingleCharString", (&CDXLibWrapper::KeyInputSingleCharString), 5, ".nnnb|i")			//string|int x,int y,int maxCharLength,bool allowCancel
.Func("KeyInputNumber", (&CDXLibWrapper::KeyInputNumber), 6, ".nnnnb|i")			//int|int x,int y,int min,int max,bool allowCancel
.Func("SetKeyInputStringColor", (&CDXLibWrapper::SetKeyInputStringColor), 2, ".s")			//void|const TCHAR* param
.Func("GetNowCount", (&CDXLibWrapper::GetNowCount), 0, NULL)			//int|
.Func("SaveDrawScreenToPNG", (&CDXLibWrapper::SaveDrawScreenToPNG), 6, ".nnnns")			//void|int leftTopX,int leftTopY,int rightBottomX,int rightBottomY,string fileName
.Func("CheckHandleAsyncLoad", (&CDXLibWrapper::CheckHandleAsyncLoad), 2, ".n")			//int|int handle
.Func("GetASyncLoadNum", (&CDXLibWrapper::GetASyncLoadNum), 0, NULL)			//int|
.Func("FileRead_open", (&CDXLibWrapper::FileRead_open), 2, ".s")			//int|string fileName
.Func("FileRead_close", (&CDXLibWrapper::FileRead_close), 2, ".n")			//void|int handle
.Func("FileRead_gets", (&CDXLibWrapper::FileRead_gets), 2, ".n")			//string|int handle
.Func("FileRead_getc", (&CDXLibWrapper::FileRead_getc), 2, ".n")			//int|int handle
.Func("FileRead_eof", (&CDXLibWrapper::FileRead_eof), 2, ".n")			//bool|int handle
.Func("ConnectNetWork", (&CDXLibWrapper::ConnectNetWork), 3, ".xn")			//int|CIPv4Address ip,int portNum
.Func("CloseNetWork", (&CDXLibWrapper::CloseNetWork), 2, ".n")			//int|int handle
.Func("PreparationListenNetWork", (&CDXLibWrapper::PreparationListenNetWork), 2, ".n")			//int|int portNum
.Func("StopListenNetWork", (&CDXLibWrapper::StopListenNetWork), 0, NULL)			//int|
.Func("NetWorkSend", (&CDXLibWrapper::NetWorkSend), 3, ".ns")			//int|int handle,const TCHAR* data
.Func("GetNetWorkDataLength", (&CDXLibWrapper::GetNetWorkDataLength), 2, ".n")			//int|int handle
.Func("GetNetWorkSendDataLength", (&CDXLibWrapper::GetNetWorkSendDataLength), 2, ".n")			//int|int handle
.Func("NetWorkRecv", (&CDXLibWrapper::NetWorkRecv), 3, ".nn")			//int|int handle,int length
.Func("NetWorkRecvToPeek", (&CDXLibWrapper::NetWorkRecvToPeek), 3, ".nn")			//int|int handle,int length
.Func("GetNewAcceptNetWork", (&CDXLibWrapper::GetNewAcceptNetWork), 0, NULL)			//int|
.Func("GetLostNetWork", (&CDXLibWrapper::GetLostNetWork), 0, NULL)			//int|
.Func("GetNetWorkAcceptState", (&CDXLibWrapper::GetNetWorkAcceptState), 2, ".n")			//int|int handle
.Func("GetNetWorkIP", (&CDXLibWrapper::GetNetWorkIP), 2, ".n")			//CIPv4Address|int handle
.Func("MakeUDPSocket", (&CDXLibWrapper::MakeUDPSocket), 2, ".n")			//int|int recvPortNum
.Func("DeleteUDPSocket", (&CDXLibWrapper::DeleteUDPSocket), 2, ".n")			//int|int UDPHandle
.Func("NetWorkSendUDP", (&CDXLibWrapper::NetWorkSendUDP), 5, ".nxns")			//int|int UDPHandle,CIPv4Address sendIP,int sendPortNum,const TCHAR* data
.Func("NetWorkRecvUDP", (&CDXLibWrapper::NetWorkRecvUDP), 3, ".nb|i")			//int|int UDPHandle,bool peekMode
.Func("CheckNetWorkRecvUDP", (&CDXLibWrapper::CheckNetWorkRecvUDP), 2, ".n")			//int|int UDPHandle
);

Sqrat::RootTable().Bind("CAddonData", Sqrat::DerivedClass<CDatabase::CAddonData, CIDValueData>()
.Var("Type", &CDatabase::CAddonData::Type)			//int
);

Sqrat::RootTable().Bind("CDBIDValues", Sqrat::DerivedClass<CDatabase::CDBCell::CDBIDValues, CNullCheck>()
.Func("Exists", (&CDatabase::CDBCell::CDBIDValues::Exists), 2, ".n")			//bool|int id
.Func("Count", (&CDatabase::CDBCell::CDBIDValues::Count), 0, NULL)			//int|
.Func("Add", (&CDatabase::CDBCell::CDBIDValues::Add), 3, ".nn")			//void|int id,int val
.Func("Insert", (&CDatabase::CDBCell::CDBIDValues::Insert), 4, ".nnn")			//void|int id,int val,int index
.Func("Delete", (&CDatabase::CDBCell::CDBIDValues::Delete), 2, ".n")			//void|int index
.Func("Clear", (&CDatabase::CDBCell::CDBIDValues::Clear), 0, NULL)			//void|
.Func("Get", (&CDatabase::CDBCell::CDBIDValues::Get), 2, ".n")			//CIDValueData*|int index
.Func("GetValue", (&CDatabase::CDBCell::CDBIDValues::GetValue), 2, ".n")			//int|int id
.Func("SetValue", (&CDatabase::CDBCell::CDBIDValues::SetValue), 3, ".nn")			//void|int id,int val
);

Sqrat::RootTable().Bind("CDBAddons", Sqrat::DerivedClass<CDatabase::CDBCell::CDBAddons, CNullCheck>()
.Func("Count", (&CDatabase::CDBCell::CDBAddons::Count), 0, NULL)			//int|
.Func("Get", (&CDatabase::CDBCell::CDBAddons::Get), 2, ".n")			//CAddonData|int index
);

Sqrat::RootTable().Bind("CDBCell", Sqrat::DerivedClass<CDatabase::CDBCell, CNullCheck>()
.Func("Init", (&CDatabase::CDBCell::Init), 0, NULL)			//void|
.Func("ToString", (&CDatabase::CDBCell::ToString), 0, NULL)			//string|
.Func("GetCellType", (&CDatabase::CDBCell::GetCellType), 0, NULL)			//int|
.Func("ConvertToColorCode", (&CDatabase::CDBCell::ConvertToColorCode), 0, NULL)			//int|
.Func("ConvertToPoint", (&CDatabase::CDBCell::ConvertToPoint), 0, NULL)			//CPoint|
.Prop("Integer", &CDatabase::CDBCell::getInteger, &CDatabase::CDBCell::setInteger)			//int
.Prop("Boolean", &CDatabase::CDBCell::getBoolean, &CDatabase::CDBCell::setBoolean)			//bool
.Prop("String", &CDatabase::CDBCell::getString, &CDatabase::CDBCell::setString)			//string
.Prop("Integers", &CDatabase::CDBCell::getIntegers)			//CIntegerArray
.Prop("IDValues", &CDatabase::CDBCell::getIDValues)			//CDBIDValues
);

Sqrat::RootTable().Bind("CDBBaseOne", Sqrat::DerivedClass<CDatabase::CDBBaseOne, CIDNameData>()
.Func("InitDB", (&CDatabase::CDBBaseOne::InitDB), 0, NULL)			//void|
.Func("InitRow", (&CDatabase::CDBBaseOne::InitRow), 2, ".n")			//void|int rowID
.Func("InitRowFromIndex", (&CDatabase::CDBBaseOne::InitRowFromIndex), 2, ".n")			//void|int rowIndex
.Func("InitCell", (&CDatabase::CDBBaseOne::InitCell), 3, ".ns")			//void|int rowID,string columnName
.Func("InitCellFromIndex", (&CDatabase::CDBBaseOne::InitCellFromIndex), 3, ".nn")			//void|int rowIndex,int columnIndex
.Func("GetRowCount", (&CDatabase::CDBBaseOne::GetRowCount), 0, NULL)			//int|
.Func("GetColumnCount", (&CDatabase::CDBBaseOne::GetColumnCount), 0, NULL)			//int|
.Func("GetColumnIndex", (&CDatabase::CDBBaseOne::GetColumnIndex), 2, ".s")			//int|string columnName
.Func("ConvertToFixedID", (&CDatabase::CDBBaseOne::ConvertToFixedID), 2, ".n")			//int|int visibleID
.Func("GetColumnName", (&CDatabase::CDBBaseOne::GetColumnName), 2, ".n")			//string|int columnIndex
.Func("GetColumnType", (&CDatabase::CDBBaseOne::GetColumnType), 2, ".n")			//string|int columnIndex
.Func("GetCell", (&CDatabase::CDBBaseOne::GetCell), 3, ".ns")			//CDatabase::CDBCell*|int rowID,string columnName
.Func("GetCellFromIndex", (&CDatabase::CDBBaseOne::GetCellFromIndex), 3, ".nn")			//CDatabase::CDBCell*|int rowIndex,int columnIndex
);

Sqrat::RootTable().Bind("CDBBase", Sqrat::DerivedClass<CDatabase::CDBBase, CNullCheck>()
.Func("GetSubDBCount", (&CDatabase::CDBBase::GetSubDBCount), 0, NULL)			//int|
.Func("GetDBFileName", (&CDatabase::CDBBase::GetDBFileName), 0, NULL)			//string|
.Func("GetSubDB", (&CDatabase::CDBBase::GetSubDB), 2, ".n")			//CDBBaseOne*|int index
);

Sqrat::RootTable().Bind("CDBSystem", Sqrat::DerivedClass<CDatabase::CDBSystem, CDatabase::CDBBase>()
.Func("GetInteger", (&CDatabase::CDBSystem::GetInteger), 2, ".s")			//int|string settingName
.Func("GetBoolean", (&CDatabase::CDBSystem::GetBoolean), 2, ".s")			//bool|string settingName
.Func("GetString", (&CDatabase::CDBSystem::GetString), 2, ".s")			//string|string settingName
.Func("GetSize", (&CDatabase::CDBSystem::GetSize), 2, ".s")			//CSize|string settingName
);

Sqrat::RootTable().Bind("CDBMaterial", Sqrat::DerivedClass<CDatabase::CDBMaterial, CDatabase::CDBBase>()
.Func("GetHandle", (&CDatabase::CDBMaterial::GetHandle), 3, ".nn")			//int|int type,int id
.Func("Play", (&CDatabase::CDBMaterial::Play), 5, ".nnnn")			//void|int soundID,int volume,int pitch,int pan
.Func("PlayForWait", (&CDatabase::CDBMaterial::PlayForWait), 5, ".nnnn")			//void|int soundID,int volume,int pitch,int pan
.Func("Stop", (&CDatabase::CDBMaterial::Stop), 2, ".n")			//void|int soundID
);

Sqrat::RootTable().Bind("CDBEffectData", Sqrat::DerivedClass<CDatabase::CDBEffect::CDBEffectData, CIDNameData>()
.Prop("Frame", &CDatabase::CDBEffect::CDBEffectData::getFrame)			//int
.Prop("TargetType", &CDatabase::CDBEffect::CDBEffectData::getTargetType)			//int
.Prop("WaitBreakFlag", &CDatabase::CDBEffect::CDBEffectData::getWaitBreakFlag, &CDatabase::CDBEffect::CDBEffectData::setWaitBreakFlag)			//bool
.Func("LoadGraph", (&CDatabase::CDBEffect::CDBEffectData::LoadGraph), 5, ".nsnn")			//void|int layerIndex,string fileName,int secSizeWidth,int secSizeHeight
.Func("Draw", (&CDatabase::CDBEffect::CDBEffectData::Draw), 10, ".nnnnnnnnn")			//void|int layerIndex,int destX,int destY,int secX,int secY,int blendMode,int trans,int ext,int angle
.Func("PlaySE", (&CDatabase::CDBEffect::CDBEffectData::PlaySE), 6, ".nnnnn")			//void|int id,int volume,int pitch,int pan,int sameCounter
.Func("Flash", (&CDatabase::CDBEffect::CDBEffectData::Flash), 5, ".nnnn")			//void|int color,int strength,int count,int sameCounter
.Func("Shake", (&CDatabase::CDBEffect::CDBEffectData::Shake), 3, ".nn")			//void|int strength,int sameCounter
.Func("WaitBreak", (&CDatabase::CDBEffect::CDBEffectData::WaitBreak), 0, NULL)			//void|
);

Sqrat::RootTable().Bind("CDBEffect", Sqrat::DerivedClass<CDatabase::CDBEffect, CDatabase::CDBBase>()
.Func("Show", (&CDatabase::CDBEffect::Show), 4, ".nnn")			//int|int id,int destX,int destY
.Func("ShowForWait", (&CDatabase::CDBEffect::ShowForWait), 4, ".nnn")			//void|int id,int destX,int destY
.Func("GetEffectData", (&CDatabase::CDBEffect::GetEffectData), 2, ".n")			//CDBEffectData*|int handle
);

Sqrat::RootTable().Bind("CTileData", Sqrat::Class<CDatabase::CDBTileset::CTileData>()
.Var("Hit", &CDatabase::CDBTileset::CTileData::Hit)			//int
.Var("InOut", &CDatabase::CDBTileset::CTileData::InOut)			//int
.Var("Priority", &CDatabase::CDBTileset::CTileData::Priority)			//int
.Var("Tag", &CDatabase::CDBTileset::CTileData::Tag)			//int
.Var("AnimPatternCount", &CDatabase::CDBTileset::CTileData::AnimPatternCount)			//int
);

Sqrat::RootTable().Bind("CTilePallet", Sqrat::Class<CDatabase::CDBTileset::CTilePallet>()
.Prop("TilesetHandle", &CDatabase::CDBTileset::CTilePallet::getTilesetHandle)			//int
.Prop("AutoTiles", &CDatabase::CDBTileset::CTilePallet::getAutoTiles)			//CIntegerArray
);

Sqrat::RootTable().Bind("CDBTileset", Sqrat::DerivedClass<CDatabase::CDBTileset, CDatabase::CDBBase>()
.Prop("ActiveTilesetID", &CDatabase::CDBTileset::getActiveTilesetID)			//int
.Func("LoadTileset", (&CDatabase::CDBTileset::LoadTileset), 2, ".n")			//CTilePallet*|int id
.Func("GetTilePallet", (&CDatabase::CDBTileset::GetTilePallet), 3, ".nn")			//CTileData*|int tileX,int tileY
.Func("GetTilePalletSize", (&CDatabase::CDBTileset::GetTilePalletSize), 0, NULL)			//CSize|
);

Sqrat::RootTable().Bind("CDBValue", Sqrat::DerivedClass<CDatabase::CDBValue, CDatabase::CDBBase>()
.Func("GetSelfValues", (&CDatabase::CDBValue::GetSelfValues), 2, ".n")			//CSelfValueData|int type
);

Sqrat::RootTable().Bind("CActorSubData", Sqrat::DerivedClass<CDatabase::CDBActorData::CActorSubData, CIDValueData>()
.Var("SelfValues", &CDatabase::CDBActorData::CActorSubData::SelfValues)			//CSelfValueData
);

Sqrat::RootTable().Bind("CActorSubDataArray", Sqrat::DerivedClass<CDatabase::CDBActorData::CActorSubDataArray, CNullCheck>()
.Func("GetIndexFromID", (&CDatabase::CDBActorData::CActorSubDataArray::GetIndexFromID), 2, ".n")			//int|int ID
.Func("GetID", (&CDatabase::CDBActorData::CActorSubDataArray::GetID), 2, ".n")			//int|int index
.Func("GetName", (&CDatabase::CDBActorData::CActorSubDataArray::GetName), 2, ".n")			//string|int index
.Func("GetValue", (&CDatabase::CDBActorData::CActorSubDataArray::GetValue), 2, ".n")			//int|int index
.Func("GetSelfValues", (&CDatabase::CDBActorData::CActorSubDataArray::GetSelfValues), 2, ".n")			//CSelfValueData*|int index
.Func("Exists", (&CDatabase::CDBActorData::CActorSubDataArray::Exists), 2, ".n")			//bool|int ID
.Func("Count", (&CDatabase::CDBActorData::CActorSubDataArray::Count), 0, NULL)			//int|
);

Sqrat::RootTable().Bind("CDBActorData", Sqrat::DerivedClass<CDatabase::CDBActorData, CIDNameData>()
.Var("Title", &CDatabase::CDBActorData::Title)			//string
.Var("IsNPC", &CDatabase::CDBActorData::IsNPC)			//bool
.Var("EXP", &CDatabase::CDBActorData::EXP)			//int
.Prop("Graphics", &CDatabase::CDBActorData::getGraphics)			//string
.Prop("GraphHandle", &CDatabase::CDBActorData::getGraphHandle)			//int
.Prop("GraphSize", &CDatabase::CDBActorData::getGraphSize)			//CSize
.Prop("Location", &CDatabase::CDBActorData::getLocation)			//CPoint
.Prop("Level", &CDatabase::CDBActorData::getLevel)			//int
.Prop("SelfValues", &CDatabase::CDBActorData::getSelfValues)			//CSelfValueData
.Prop("PotentialSkills", &CDatabase::CDBActorData::getPotentialSkills)			//CActorSubDataArray
.Prop("Attributes", &CDatabase::CDBActorData::getAttributes)			//CActorSubDataArray
.Prop("Classes", &CDatabase::CDBActorData::getClasses)			//CActorSubDataArray
.Prop("Equips", &CDatabase::CDBActorData::getEquips)			//CActorSubDataArray
.Prop("States", &CDatabase::CDBActorData::getStates)			//CActorSubDataArray
.Prop("LearnedSkills", &CDatabase::CDBActorData::getLearnedSkills)			//CActorSubDataArray
.Prop("LevelUpParameters", &CDatabase::CDBActorData::getLevelUpParameters)			//CActorSubDataArray
.Prop("LevelUpSkills", &CDatabase::CDBActorData::getLevelUpSkills)			//CActorSubDataArray
.Func("PrintData", (&CDatabase::CDBActorData::PrintData), 0, NULL)			//void|
.StaticFunc("CreateActorFromChar", (&CDatabase::CDBActorData::CreateActorFromChar), 2, ".n")			//CDBActorData|int ID
.StaticFunc("CreateActorFromEnemy", (&CDatabase::CDBActorData::CreateActorFromEnemy), 2, ".n")			//CDBActorData|int ID
.Func("LoadGraphics", (&CDatabase::CDBActorData::LoadGraphics), 2, ".s")			//void|string fileName
.Func("DisposeGraphics", (&CDatabase::CDBActorData::DisposeGraphics), 0, NULL)			//void|
.Func("GetBaseFDBIndex", (&CDatabase::CDBActorData::GetBaseFDBIndex), 0, NULL)			//int|
.Func("SetClass", (&CDatabase::CDBActorData::SetClass), 2, ".n")			//void|int ID
.Func("SetEquip", (&CDatabase::CDBActorData::SetEquip), 3, ".nb|i")			//bool|int ID,bool absolute = false
.Func("SetSkill", (&CDatabase::CDBActorData::SetSkill), 2, ".n")			//void|int ID
.Func("SetState", (&CDatabase::CDBActorData::SetState), 3, ".nb|i")			//bool|int ID,bool absolute
.Func("ReleaseClass", (&CDatabase::CDBActorData::ReleaseClass), 2, ".n")			//bool|int ID
.Func("ReleaseEquip", (&CDatabase::CDBActorData::ReleaseEquip), 3, ".nb|i")			//bool|int ID,bool absolute = false
.Func("ReleaseEquipType", (&CDatabase::CDBActorData::ReleaseEquipType), 3, ".nb|i")			//bool|int itemTypeID,bool absolute = false
.Func("ReleaseSkill", (&CDatabase::CDBActorData::ReleaseSkill), 2, ".n")			//bool|int ID
.Func("ReleaseState", (&CDatabase::CDBActorData::ReleaseState), 2, ".n")			//bool|int ID
.Func("ClearClass", (&CDatabase::CDBActorData::ClearClass), 0, NULL)			//void|
.Func("ClearEquip", (&CDatabase::CDBActorData::ClearEquip), 2, ".b|i")			//void|bool absolute = false
.Func("ClearSkill", (&CDatabase::CDBActorData::ClearSkill), 0, NULL)			//void|
.Func("ClearState", (&CDatabase::CDBActorData::ClearState), 0, NULL)			//void|
.Func("ClearStateOnlyBad", (&CDatabase::CDBActorData::ClearStateOnlyBad), 0, NULL)			//void|
.Func("GetParameter", (&CDatabase::CDBActorData::GetParameter), 2, ".n")			//CIDValueData*|int ID
.Func("GetAttributeResistance", (&CDatabase::CDBActorData::GetAttributeResistance), 2, ".n")			//int|int ID
.Func("GetStateResistance", (&CDatabase::CDBActorData::GetStateResistance), 2, ".n")			//int|int ID
.Func("GetEquipIDFromItemTypeID", (&CDatabase::CDBActorData::GetEquipIDFromItemTypeID), 2, ".n")			//int|int itemTypeID
.Func("CheckAttribute", (&CDatabase::CDBActorData::CheckAttribute), 2, ".n")			//bool|int attrID
.Func("CheckStateLimit", (&CDatabase::CDBActorData::CheckStateLimit), 2, ".n")			//bool|int stateLimit
.Func("CheckUseSkill", (&CDatabase::CDBActorData::CheckUseSkill), 2, ".n")			//bool|int skillID
.Func("CheckUseSkillType", (&CDatabase::CDBActorData::CheckUseSkillType), 2, ".n")			//bool|int skillTypeID
.Func("CheckEquipLock", (&CDatabase::CDBActorData::CheckEquipLock), 2, ".n")			//bool|int itemTypeID
.Func("CheckEquip", (&CDatabase::CDBActorData::CheckEquip), 2, ".n")			//bool|int itemID
.Func("PlusEXP", (&CDatabase::CDBActorData::PlusEXP), 2, ".n")			//int|int exp
.Func("LevelUp", (&CDatabase::CDBActorData::LevelUp), 2, ".n")			//int|int val
.Func("GetNextEXP", (&CDatabase::CDBActorData::GetNextEXP), 0, NULL)			//int|
);

Sqrat::RootTable().Bind("CDBActor", Sqrat::DerivedClass<CDatabase::CDBActor, CDatabase::CDBBase>()
.Func("GetCount", (&CDatabase::CDBActor::GetCount), 0, NULL)			//int|
.Func("InitData", (&CDatabase::CDBActor::InitData), 2, ".n")			//void|int ID
.Func("InitDataFromIndex", (&CDatabase::CDBActor::InitDataFromIndex), 2, ".n")			//void|int index
.Func("GetData", (&CDatabase::CDBActor::GetData), 2, ".n")			//CDBActorData*|int ID
.Func("GetDataFromIndex", (&CDatabase::CDBActor::GetDataFromIndex), 2, ".n")			//CDBActorData*|int index
);

Sqrat::RootTable().Bind("CDBParty", Sqrat::DerivedClass<CDatabase::CDBParty, CDatabase::CDBBase>()
.Func("ClearMembers", (&CDatabase::CDBParty::ClearMembers), 0, NULL)			//void|
.Func("ClearItems", (&CDatabase::CDBParty::ClearItems), 0, NULL)			//void|
.Func("AddMember", (&CDatabase::CDBParty::AddMember), 3, ".nn")			//void|int id,int index
.Func("DeleteMemberFromID", (&CDatabase::CDBParty::DeleteMemberFromID), 2, ".n")			//void|int id
.Func("DeleteMemberFromIndex", (&CDatabase::CDBParty::DeleteMemberFromIndex), 2, ".n")			//void|int index
.Func("CheckCharInParty", (&CDatabase::CDBParty::CheckCharInParty), 2, ".n")			//int|int id
.Func("GetCharIDFromPartyIndex", (&CDatabase::CDBParty::GetCharIDFromPartyIndex), 2, ".n")			//int|int index
.Func("GetCharCount", (&CDatabase::CDBParty::GetCharCount), 0, NULL)			//int|
.Func("AddItem", (&CDatabase::CDBParty::AddItem), 3, ".nn")			//void|int id,int count
.Func("DeleteItem", (&CDatabase::CDBParty::DeleteItem), 3, ".nn")			//void|int id,int count
.Func("GetItemDataFromIndex", (&CDatabase::CDBParty::GetItemDataFromIndex), 2, ".n")			//CIDValueData*|int index
.Func("GetItemCountAll", (&CDatabase::CDBParty::GetItemCountAll), 0, NULL)			//int|
.Func("GetItemTypeCount", (&CDatabase::CDBParty::GetItemTypeCount), 2, ".n")			//int|int itemTypeID
.Func("GetItemCount", (&CDatabase::CDBParty::GetItemCount), 2, ".n")			//int|int itemID
.Func("SortItemsByVisibleID", (&CDatabase::CDBParty::SortItemsByVisibleID), 2, ".b|i")			//void|bool reverse
.Func("SortItemsByName", (&CDatabase::CDBParty::SortItemsByName), 2, ".b|i")			//void|bool reverse
.Prop("Money", &CDatabase::CDBParty::getMoney, &CDatabase::CDBParty::setMoney)			//int
);

Sqrat::RootTable().Bind("CDBUser", Sqrat::DerivedClass<CDatabase::CDBUser, CDatabase::CDBBase>()
.Func("GetUserDB", (&CDatabase::CDBUser::GetUserDB), 2, ".n")			//CDBBaseOne*|int id
);

Sqrat::RootTable().Bind("CDatabase", Sqrat::Class<CDatabase>()
.Prop("UDB", &CDatabase::getUDB)			//CDBUser
.Func("GetDB", (&CDatabase::GetDB), 3, ".nn")			//CDBBase*|int dbType,int dbIndex
.Prop("FDBEffect", &CDatabase::getFDBEffect)			//CDBEffect
.Prop("FDBMaterial", &CDatabase::getFDBMaterial)			//CDBMaterial
.Prop("FDBSystem", &CDatabase::getFDBSystem)			//CDBSystem
.Prop("FDBTileset", &CDatabase::getFDBTileset)			//CDBTileset
.Prop("VDBValue", &CDatabase::getVDBValue)			//CDBValue
.Prop("VDBParty", &CDatabase::getVDBParty)			//CDBParty
.Prop("VDBActor", &CDatabase::getVDBActor)			//CDBActor
.Func("GetCell", (&CDatabase::GetCell), 6, ".nnnns")			//CDatabase::CDBCell*|int dbType,int dbIndex,int subDB,int rowID,string columnName
.Func("GetCellFromIndex", (&CDatabase::GetCellFromIndex), 6, ".nnnnn")			//CDatabase::CDBCell*|int dbType,int dbIndex,int subDB,int rowIndex,int columnIndex
.Func("InitDBType", (&CDatabase::InitDBType), 2, ".n")			//void|int dbType
.Func("InitDB", (&CDatabase::InitDB), 4, ".nnn")			//void|int dbType,int dbIndex,int subDB
.Func("InitRow", (&CDatabase::InitRow), 5, ".nnnn")			//void|int dbType,int dbIndex,int subDB,int rowID
.Func("InitCell", (&CDatabase::InitCell), 6, ".nnnns")			//void|int dbType,int dbIndex,int subDB,int rowID,string columnName
.Func("InitCellFromIndex", (&CDatabase::InitCellFromIndex), 6, ".nnnnn")			//void|int dbType,int dbIndex,int subDB,int rowIndex,int columnIndex
);

Sqrat::RootTable().Bind("CMaterialBase", Sqrat::DerivedClass<CMaterialBase, CNullCheck, Sqrat::NoConstructor>()
);

Sqrat::RootTable().Bind("CMaterialGraphics", Sqrat::DerivedClass<CMaterial::CMaterialGraphics, CMaterialBase>()
.Func("Add", ((int(CMaterial::CMaterialGraphics::*)(string))&CMaterial::CMaterialGraphics::Add), 2, ".s")			//int|string fileName
.Func("Delete", (&CMaterial::CMaterialGraphics::Delete), 2, ".n")			//void|int hdl
.Func("Clear", (&CMaterial::CMaterialGraphics::Clear), 0, NULL)			//void|
.Prop("IconSize", &CMaterial::CMaterialGraphics::getIconSize)			//int
.Prop("IconGraphicHandle", &CMaterial::CMaterialGraphics::getIconGraphicHandle)			//int
);

Sqrat::RootTable().Bind("CMaterialSound", Sqrat::DerivedClass<CMaterial::CMaterialSound, CMaterialBase>()
.Prop("StandardVolume", &CMaterial::CMaterialSound::getStandardVolume, &CMaterial::CMaterialSound::setStandardVolume)			//int
.Func("Add", ((int(CMaterial::CMaterialSound::*)(string))&CMaterial::CMaterialSound::Add), 2, ".s")			//int|string fileName
.Func("Delete", (&CMaterial::CMaterialSound::Delete), 2, ".n")			//void|int hdl
.Func("Clear", (&CMaterial::CMaterialSound::Clear), 0, NULL)			//void|
.Func("Play", (&CMaterial::CMaterialSound::Play), 7, ".nnnnnn")			//void|int hdl,int playType,int volume = 255,int pitchRate = 0,int panPot = 0,double fadeTime = 0.0F
.Func("PlayOnce", (&CMaterial::CMaterialSound::PlayOnce), 6, ".nnnnn")			//void|int hdl,int playType,int volume = 255,int pitchRate = 0,int panPot = 0
.Func("Stop", (&CMaterial::CMaterialSound::Stop), 4, ".nnb|i")			//void|int hdl,double fadeTime = 0.0F,bool afterDelete = false
.Func("StopAll", (&CMaterial::CMaterialSound::StopAll), 3, ".nb|i")			//void|double fadeTime = 0.0F,bool afterDelete = false
.Func("Pause", (&CMaterial::CMaterialSound::Pause), 3, ".nn")			//void|int hdl,double fadeTime = 0.0F
.Func("Resume", (&CMaterial::CMaterialSound::Resume), 4, ".nb|in")			//void|int hdl,bool loopMode = false,double fadeTime = 0.0F
.Func("CheckPlayStatus", (&CMaterial::CMaterialSound::CheckPlayStatus), 2, ".n")			//int|int hdl
);

Sqrat::RootTable().Bind("CMaterialFont", Sqrat::DerivedClass<CMaterial::CMaterialFont, CMaterialBase>()
.Func("Add", ((int(CMaterial::CMaterialFont::*)(int, string, string, int, int, int, int, int, int))&CMaterial::CMaterialFont::Add), 10, ".nssnnnnnn")			//int|int dbID,string fontName,string fileName,int size,int thick,int lineSpace,int space,int type,int edgeColor
.Func("Delete", (&CMaterial::CMaterialFont::Delete), 2, ".n")			//void|int hdl
.Func("Clear", (&CMaterial::CMaterialFont::Clear), 0, NULL)			//void|
);

Sqrat::RootTable().Bind("CMaterial", Sqrat::Class<CMaterial>()
.Func("Clear", (&CMaterial::Clear), 0, NULL)			//void|
.Prop("Fonts", &CMaterial::getFonts)			//CMaterialFont
.Prop("Graphics", &CMaterial::getGraphics)			//CMaterialGraphics
.Prop("Sounds", &CMaterial::getSounds)			//CMaterialSound
);

Sqrat::RootTable().Bind("CSceneBase", Sqrat::DerivedClass<CSceneBase, CNullCheck, Sqrat::NoConstructor>()
.Prop("SubSceneID", &CSceneBase::getSubSceneID)			//int
.Prop("BGM", &CSceneBase::getBGM)			//CSoundData
.Var("BeforeBGMFileName", &CSceneBase::BeforeBGMFileName)			//string
.Func("PlayBGM", (&CSceneBase::PlayBGM), 5, ".snnn")			//void|string fileName,int volume,int pitch,int pan
.Prop("BGS", &CSceneBase::getBGS)			//CSoundData
.Var("BeforeBGSFileName", &CSceneBase::BeforeBGSFileName)			//string
.Func("PlayBGS", (&CSceneBase::PlayBGS), 5, ".snnn")			//void|string fileName,int volume,int pitch,int pan
.Func("GetName", (&CSceneBase::GetName), 0, NULL)			//string|
);

Sqrat::RootTable().Bind("CSceneTitle", Sqrat::DerivedClass<CSceneTitle, CSceneBase>()
);

Sqrat::RootTable().Bind("CEventMoveTask", Sqrat::Class<CSceneMap::CMapData::CEventMoveTask>()
.Var("TaskType", &CSceneMap::CMapData::CEventMoveTask::TaskType)			//int
.Var("OptionInteger", &CSceneMap::CMapData::CEventMoveTask::OptionInteger)			//int
.Var("OptionString", &CSceneMap::CMapData::CEventMoveTask::OptionString)			//string
);

Sqrat::RootTable().Bind("CMapObjectData", Sqrat::DerivedClass<CSceneMap::CMapData::CMapObjectData, CIDNameData>()
.Prop("Position", &CSceneMap::CMapData::CMapObjectData::getPosition)			//CPoint
.Prop("PositionOnPixel", &CSceneMap::CMapData::CMapObjectData::getPositionOnPixel)			//CPoint
.Prop("MoveCounter", &CSceneMap::CMapData::CMapObjectData::getMoveCounter)			//CPoint
.Var("Hide", &CSceneMap::CMapData::CMapObjectData::Hide)			//bool
.Var("ForbiddenAutoMove", &CSceneMap::CMapData::CMapObjectData::ForbiddenAutoMove)			//bool
.Var("MoveWaitTimer", &CSceneMap::CMapData::CMapObjectData::MoveWaitTimer)			//int
.Func("DoExtraMove", (&CSceneMap::CMapData::CMapObjectData::DoExtraMove), 2, ".b|i")			//void|bool waitMode
.Func("AutoSetMoveCounter", (&CSceneMap::CMapData::CMapObjectData::AutoSetMoveCounter), 2, ".n")			//void|int direction4
.Prop("IsMoving", &CSceneMap::CMapData::CMapObjectData::getIsMoving)			//bool
.Func("SetExtraMove", (&CSceneMap::CMapData::CMapObjectData::SetExtraMove), 4, ".nns")			//CMapObjectData*|int type,int par1,string par2
.Func("ApplyMapLoopPosition", (&CSceneMap::CMapData::CMapObjectData::ApplyMapLoopPosition), 0, NULL)			//void|
);

Sqrat::RootTable().Bind("CMapCharObjectData", Sqrat::DerivedClass<CSceneMap::CMapData::CMapCharObjectData, CNullCheck>()
.Var("MoveSpeed", &CSceneMap::CMapData::CMapCharObjectData::MoveSpeed)			//int
.Var("AnimID", &CSceneMap::CMapData::CMapCharObjectData::AnimID)			//int
.Var("EnabledStopAnim", &CSceneMap::CMapData::CMapCharObjectData::EnabledStopAnim)			//bool
.Var("EnabledWalkAnim", &CSceneMap::CMapData::CMapCharObjectData::EnabledWalkAnim)			//bool
.Var("NoHit", &CSceneMap::CMapData::CMapCharObjectData::NoHit)			//bool
.Var("FixedDirection", &CSceneMap::CMapData::CMapCharObjectData::FixedDirection)			//bool
.Var("OneCharSize", &CSceneMap::CMapData::CMapCharObjectData::OneCharSize)			//CSize
.Prop("Graphics", &CSceneMap::CMapData::CMapCharObjectData::getGraphics)			//string
.Prop("Direction", &CSceneMap::CMapData::CMapCharObjectData::getDirection, &CSceneMap::CMapData::CMapCharObjectData::setDirection)			//int
.Prop("GraphHandle", &CSceneMap::CMapData::CMapCharObjectData::getGraphHandle)			//int
.Func("LoadGraphics", (&CSceneMap::CMapData::CMapCharObjectData::LoadGraphics), 2, ".s")			//void|string fileName
);

Sqrat::RootTable().Bind("CMapPlayer", Sqrat::Class<CSceneMap::CMapData::CMapPlayer>()
.Prop("MapObject", &CSceneMap::CMapData::CMapPlayer::getMapObject)			//CMapObjectData
.Prop("CharData", &CSceneMap::CMapData::CMapPlayer::getCharData)			//CMapCharObjectData
);

Sqrat::RootTable().Bind("CMapEventPageData", Sqrat::DerivedClass<CSceneMap::CMapData::CMapEventData::CMapEventPageData, CSceneMap::CMapData::CMapCharObjectData>()
.Var("TriggerType", &CSceneMap::CMapData::CMapEventData::CMapEventPageData::TriggerType)			//int
.Var("MoveType", &CSceneMap::CMapData::CMapEventData::CMapEventPageData::MoveType)			//int
.Var("MoveFrequency", &CSceneMap::CMapData::CMapEventData::CMapEventPageData::MoveFrequency)			//int
.Var("DrawPriority", &CSceneMap::CMapData::CMapEventData::CMapEventPageData::DrawPriority)			//int
.Var("TilePalletPosition", &CSceneMap::CMapData::CMapEventData::CMapEventPageData::TilePalletPosition)			//CPoint
.Prop("IsTilesetGraphics", &CSceneMap::CMapData::CMapEventData::CMapEventPageData::getIsTilesetGraphics)			//bool
.Prop("HitRect", &CSceneMap::CMapData::CMapEventData::CMapEventPageData::getHitRect)			//CSize
.Func("LoadGraphics", (&CSceneMap::CMapData::CMapEventData::CMapEventPageData::LoadGraphics), 2, ".s")			//void|string fileName
);

Sqrat::RootTable().Bind("CMapEventData", Sqrat::DerivedClass<CSceneMap::CMapData::CMapEventData, CSceneMap::CMapData::CMapObjectData>()
.Prop("IsDummy", &CSceneMap::CMapData::CMapEventData::getIsDummy)			//bool
.Var("VisibleID", &CSceneMap::CMapData::CMapEventData::VisibleID)			//int
.Prop("SelfValues", &CSceneMap::CMapData::CMapEventData::getSelfValues)			//CSelfValueData
.Func("SetPageIndex", (&CSceneMap::CMapData::CMapEventData::SetPageIndex), 2, ".n")			//void|int val
.Func("GetPageIndex", (&CSceneMap::CMapData::CMapEventData::GetPageIndex), 0, NULL)			//int|
.Func("GetPageCount", (&CSceneMap::CMapData::CMapEventData::GetPageCount), 0, NULL)			//int|
.Func("AddPage", (&CSceneMap::CMapData::CMapEventData::AddPage), 2, ".x")			//void|CMapEventPageData pg
.Func("AddMoveTask", (&CSceneMap::CMapData::CMapEventData::AddMoveTask), 4, ".nns")			//CMapEventData*|int type,int par1,string par2
.Func("SetDirection4", (&CSceneMap::CMapData::CMapEventData::SetDirection4), 2, ".n")			//void|int dir4
.Func("SetDirection4ToPlayer", (&CSceneMap::CMapData::CMapEventData::SetDirection4ToPlayer), 0, NULL)			//void|
.Func("SetDirection4AgainstPlayer", (&CSceneMap::CMapData::CMapEventData::SetDirection4AgainstPlayer), 0, NULL)			//void|
.Func("GetDirection4ToPlayer", (&CSceneMap::CMapData::CMapEventData::GetDirection4ToPlayer), 0, NULL)			//int|
.Prop("RouteLock", &CSceneMap::CMapData::CMapEventData::getRouteLock)			//bool
.Prop("CurrentPage", &CSceneMap::CMapData::CMapEventData::getCurrentPage)			//CMapEventPageData
);

Sqrat::RootTable().Bind("CMapData", Sqrat::Class<CSceneMap::CMapData>()
.Prop("TileSize", &CSceneMap::CMapData::getTileSize)			//int
.Prop("ShadowTransparent", &CSceneMap::CMapData::getShadowTransparent)			//int
.Prop("FogTransparent", &CSceneMap::CMapData::getFogTransparent)			//int
.Prop("MapPixelOffset", &CSceneMap::CMapData::getMapPixelOffset)			//CPoint
.Prop("TilePallet", &CSceneMap::CMapData::getTilePallet)			//CDatabase::CDBTileset::CTilePallet
.Var("GUID", &CSceneMap::CMapData::GUID)			//string
.Prop("MapSize", &CSceneMap::CMapData::getMapSize)			//CSize
.Var("MapLoopOption", &CSceneMap::CMapData::MapLoopOption)			//int
.Prop("TilesetID", &CSceneMap::CMapData::getTilesetID)			//int
.Prop("MoveSpeed", &CSceneMap::CMapData::getMoveSpeed)			//int
.Prop("BaseMapFileName", &CSceneMap::CMapData::getBaseMapFileName)			//string
.Var("CurrentMapName", &CSceneMap::CMapData::CurrentMapName)			//string
.Prop("CurrentMapFileName", &CSceneMap::CMapData::getCurrentMapFileName)			//string
.Prop("FirstMapBGMRawString", &CSceneMap::CMapData::getFirstMapBGMRawString)			//string
.Prop("FirstMapBGSRawString", &CSceneMap::CMapData::getFirstMapBGSRawString)			//string
.Prop("BackHandle", &CSceneMap::CMapData::getBackHandle)			//int
.Prop("CurrentMapBackFileName", &CSceneMap::CMapData::getCurrentMapBackFileName)			//string
.Var("BackScrollType", &CSceneMap::CMapData::BackScrollType)			//int
.Var("BackScrollSpeed", &CSceneMap::CMapData::BackScrollSpeed)			//int
.Prop("FogHandle", &CSceneMap::CMapData::getFogHandle)			//int
.Prop("CurrentFogFileName", &CSceneMap::CMapData::getCurrentFogFileName)			//string
.Var("FogScrollType", &CSceneMap::CMapData::FogScrollType)			//int
.Var("FogScrollSpeed", &CSceneMap::CMapData::FogScrollSpeed)			//int
.Var("FogBlend", &CSceneMap::CMapData::FogBlend)			//int
.Prop("Units", &CSceneMap::CMapData::getUnits)			//CIntegerArray
.Var("EncounterRate", &CSceneMap::CMapData::EncounterRate)			//int
.Prop("BattleBackFileName", &CSceneMap::CMapData::getBattleBackFileName)			//string
.Prop("Player", &CSceneMap::CMapData::getPlayer)			//CMapPlayer
.Prop("AnimCounter", &CSceneMap::CMapData::getAnimCounter, &CSceneMap::CMapData::setAnimCounter)			//int
.Func("LoadMap", (&CSceneMap::CMapData::LoadMap), 3, ".sb|i")			//void|string fileName,bool exceptEV
.Func("AfterLoadMap", (&CSceneMap::CMapData::AfterLoadMap), 0, NULL)			//void|
.Func("GetTile", (&CSceneMap::CMapData::GetTile), 4, ".nnn")			//CPoint|int layer,int x,int y
.Func("SetTile", (&CSceneMap::CMapData::SetTile), 7, ".nnnnnb|i")			//void|int layer,int x,int y,int tilex,int tiley,bool absolute
.Func("GetQuarterTile", (&CSceneMap::CMapData::GetQuarterTile), 5, ".nnnn")			//int|int layer,int x,int y,int q
.Func("SetQuarterTile", (&CSceneMap::CMapData::SetQuarterTile), 6, ".nnnnn")			//void|int layer,int x,int y,int q,int val
.Func("IsWalkableTile", (&CSceneMap::CMapData::IsWalkableTile), 3, ".nn")			//bool|int x,int y
.Func("CheckTileGroupNum", (&CSceneMap::CMapData::CheckTileGroupNum), 4, ".nnn")			//int|int x,int y,int groupNum
.Func("GetTileGroupNum", (&CSceneMap::CMapData::GetTileGroupNum), 4, ".nnn")			//int|int layer,int x,int y
.Func("DrawMap", (&CSceneMap::CMapData::DrawMap), 2, ".n")			//void|int tileAnimLoopFrame
.Func("LoadBack", (&CSceneMap::CMapData::LoadBack), 2, ".s")			//void|string fileName
.Func("DrawBack", (&CSceneMap::CMapData::DrawBack), 0, NULL)			//void|
.Func("LoadFog", (&CSceneMap::CMapData::LoadFog), 2, ".s")			//void|string fileName
.Func("DrawFog", (&CSceneMap::CMapData::DrawFog), 0, NULL)			//void|
.Func("EVCount", (&CSceneMap::CMapData::EVCount), 0, NULL)			//int|
.Func("EVReservedCount", (&CSceneMap::CMapData::EVReservedCount), 0, NULL)			//int|
.Func("GetEVFixedID", (&CSceneMap::CMapData::GetEVFixedID), 2, ".n")			//int|int visibleID
.Func("EVClear", (&CSceneMap::CMapData::EVClear), 0, NULL)			//void|
.Func("EVDeleteFromID", (&CSceneMap::CMapData::EVDeleteFromID), 2, ".n")			//void|int id
.Func("EVExists", (&CSceneMap::CMapData::EVExists), 2, ".n")			//bool|int id
.Func("EVGetData", (&CSceneMap::CMapData::EVGetData), 2, ".n")			//CMapEventData*|int id
.Func("EVGetDataFromIndex", (&CSceneMap::CMapData::EVGetDataFromIndex), 2, ".n")			//CMapEventData*|int idx
.Func("ReserveEV", (&CSceneMap::CMapData::ReserveEV), 2, ".n")			//void|int EVID
.Func("RunEV", ((bool(CSceneMap::CMapData::*)(int, int, bool))&CSceneMap::CMapData::RunEV), 4, ".nnb|i")			//bool|int EVID,int type,bool pushRunningStack
.Func("SetEV", (&CSceneMap::CMapData::SetEV), 6, ".nnnns")			//void|int EVID,int visibleID,int x,int y,string name
.Func("AddEVPage", (&CSceneMap::CMapData::AddEVPage), 4, ".nss")			//void|int EVID,string param,string graph
.Func("SetDummyEV", (&CSceneMap::CMapData::SetDummyEV), 10, ".nnsnnnb|ib|ib|i")			//int|int x,int y,string graph,int direction4,int animID,int priority,bool enabledStopAnim,bool enabledWalkAnim,bool fixedDirection
.Prop("IsEVRunning", &CSceneMap::CMapData::getIsEVRunning)			//bool
.Func("ClearDummyEVs", (&CSceneMap::CMapData::ClearDummyEVs), 0, NULL)			//void|
.Prop("ThisEVData", &CSceneMap::CMapData::getThisEVData)			//CMapEventData
.Func("CheckBeforeMove", (&CSceneMap::CMapData::CheckBeforeMove), 6, ".nnnnb|i")			//bool|int EVID,int x,int y,int direction4,bool EVTrigger
.Func("CheckOverlapTriggerEV", (&CSceneMap::CMapData::CheckOverlapTriggerEV), 4, ".nnn")			//bool|int EVID,int x,int y
.Func("CheckPressEnterTriggerEV", (&CSceneMap::CMapData::CheckPressEnterTriggerEV), 0, NULL)			//bool|
);

Sqrat::RootTable().Bind("CShopData", Sqrat::Class<CSceneMap::CShopData>()
.Func("Add", (&CSceneMap::CShopData::Add), 2, ".n")			//void|int id
.Func("Clear", (&CSceneMap::CShopData::Clear), 0, NULL)			//void|
.Func("GetCount", (&CSceneMap::CShopData::GetCount), 0, NULL)			//int|
.Func("Get", (&CSceneMap::CShopData::Get), 2, ".n")			//CIDValueData|int index
.Func("SortItemsByVisibleID", (&CSceneMap::CShopData::SortItemsByVisibleID), 2, ".b|i")			//void|bool reverse
.Func("SortItemsByName", (&CSceneMap::CShopData::SortItemsByName), 2, ".b|i")			//void|bool reverse
.Func("SortItemsByPrice", (&CSceneMap::CShopData::SortItemsByPrice), 2, ".b|i")			//void|bool reverse
);

Sqrat::RootTable().Bind("CSceneMap", Sqrat::DerivedClass<CSceneMap, CSceneBase>()
.Prop("MapData", &CSceneMap::getMapData)			//CMapData
.Prop("ShopData", &CSceneMap::getShopData)			//CShopData
.Func("ShowMessage", (&CSceneMap::ShowMessage), 4, ".nss")			//void|int faceID,string speakerName,string msg
.Func("ChangeMap", (&CSceneMap::ChangeMap), 5, ".snnn")			//void|string fileName,int x,int y,int direction4
.StaticFunc("TurnLeft4", (&CSceneMap::TurnLeft4), 2, ".n")			//int|int direction4
.StaticFunc("TurnLeft8", (&CSceneMap::TurnLeft8), 2, ".n")			//int|int direction8
.StaticFunc("TurnRight4", (&CSceneMap::TurnRight4), 2, ".n")			//int|int direction4
.StaticFunc("TurnRight8", (&CSceneMap::TurnRight8), 2, ".n")			//int|int direction8
.StaticFunc("TurnHalf4", (&CSceneMap::TurnHalf4), 2, ".n")			//int|int direction4
.StaticFunc("TurnHalf8", (&CSceneMap::TurnHalf8), 2, ".n")			//int|int direction8
.StaticFunc("PointToDirection4", (&CSceneMap::PointToDirection4), 2, ".x")			//int|const CPoint& pos
.StaticFunc("PointToDirection8", (&CSceneMap::PointToDirection8), 2, ".x")			//int|const CPoint& pos
.StaticFunc("Direction4ToPoint", (&CSceneMap::Direction4ToPoint), 2, ".n")			//CPoint|int direction4
.StaticFunc("Direction8ToPoint", (&CSceneMap::Direction8ToPoint), 2, ".n")			//CPoint|int direction8
);

Sqrat::RootTable().Bind("CThreadInfo", Sqrat::DerivedClass<CMultiThread::CThreadInfo, CNullCheck>()
.Prop("Name", &CMultiThread::CThreadInfo::getName)			//string
.Prop("SQFuncName", &CMultiThread::CThreadInfo::getSQFuncName)			//string
.Prop("Suspended", &CMultiThread::CThreadInfo::getSuspended, &CMultiThread::CThreadInfo::setSuspended)			//bool
.Prop("Protected", &CMultiThread::CThreadInfo::getProtected, &CMultiThread::CThreadInfo::setProtected)			//bool
.Prop("Handle", &CMultiThread::CThreadInfo::getHandle)			//int
.Func("AddValueInteger", (&CMultiThread::CThreadInfo::AddValueInteger), 3, ".sn")			//CThreadInfo*|string name,int Value
.Func("AddValueDecimal", (&CMultiThread::CThreadInfo::AddValueDecimal), 3, ".sn")			//CThreadInfo*|string name,double Value
.Func("AddValueString", (&CMultiThread::CThreadInfo::AddValueString), 3, ".ss")			//CThreadInfo*|string name,string Value
.Func("SetValueInteger", (&CMultiThread::CThreadInfo::SetValueInteger), 3, ".sn")			//void|string name,int Value
.Func("SetValueDecimal", (&CMultiThread::CThreadInfo::SetValueDecimal), 3, ".sn")			//void|string name,double Value
.Func("SetValueString", (&CMultiThread::CThreadInfo::SetValueString), 3, ".ss")			//void|string name,string Value
.Func("GetValueInteger", (&CMultiThread::CThreadInfo::GetValueInteger), 2, ".s")			//int|string name
.Func("GetValueDecimal", (&CMultiThread::CThreadInfo::GetValueDecimal), 2, ".s")			//double|string name
.Func("GetValueString", (&CMultiThread::CThreadInfo::GetValueString), 2, ".s")			//string|string name
);

Sqrat::RootTable().Bind("CMultiThread", Sqrat::Class<CMultiThread>()
.Func("AddThread", ((CMultiThread::CThreadInfo*(CMultiThread::*)(string, string, int, bool))&CMultiThread::AddThread), 5, ".ssnb|i")			//CMultiThread::CThreadInfo*|string name,string funcName,int enableSceneID,bool isProtected
.Func("GetThreadCount", (&CMultiThread::GetThreadCount), 0, NULL)			//int|
.Func("CountThread", (&CMultiThread::CountThread), 2, ".s")			//int|string name
.Func("GetThread", (&CMultiThread::GetThread), 2, ".n")			//CThreadInfo*|int handle
.Func("Clear", (&CMultiThread::Clear), 2, ".b|i")			//void|bool LeaveCurrentScene
.Func("DeleteThread", (&CMultiThread::DeleteThread), 2, ".s")			//void|string name
.Func("SetSuspend", (&CMultiThread::SetSuspend), 3, ".sb|i")			//void|string name,bool IsSuspended
.Func("SetProtect", (&CMultiThread::SetProtect), 3, ".sb|i")			//void|string name,bool IsProtected
.Func("GetSuspend", (&CMultiThread::GetSuspend), 2, ".s")			//bool|string name
.Func("GetProtect", (&CMultiThread::GetProtect), 2, ".s")			//bool|string name
.Func("WaitForThread", (&CMultiThread::WaitForThread), 3, ".sn")			//void|string name,double MaxSec
);

Sqrat::RootTable().Bind("CFPS", Sqrat::Class<CFPS>()
.Prop("SkipFrameCount", &CFPS::getSkipFrameCount, &CFPS::setSkipFrameCount)			//int
.Func("Update", (&CFPS::Update), 0, NULL)			//void|
);

Sqrat::ConstTable()
.Enum("DXInterpolation", Sqrat::Enumeration()			//TS::System::DXInterpolation
.Const("Nearest", (int)TS::System::DXInterpolation::Nearest)
.Const("Bilinear", (int)TS::System::DXInterpolation::Bilinear)
);

Sqrat::ConstTable()
.Enum("BlendMode", Sqrat::Enumeration()			//TS::System::BlendMode
.Const("Alpha", (int)TS::System::BlendMode::Alpha)
.Const("Add", (int)TS::System::BlendMode::Add)
.Const("Subtract", (int)TS::System::BlendMode::Subtract)
.Const("Multiply", (int)TS::System::BlendMode::Multiply)
.Const("Invsrc", (int)TS::System::BlendMode::Invsrc)
);

Sqrat::ConstTable()
.Enum("DBDefaultColumnIndices", Sqrat::Enumeration()			//TS::DB::DBDefaultColumnIndices
.Const("FixedID", (int)TS::DB::DBDefaultColumnIndices::FixedID)
.Const("VisibleID", (int)TS::DB::DBDefaultColumnIndices::VisibleID)
.Const("Name", (int)TS::DB::DBDefaultColumnIndices::Name)
);

Sqrat::ConstTable()
.Enum("DBCurveDataIndices", Sqrat::Enumeration()			//TS::DB::DBCurveDataIndices
.Const("IDName", (int)TS::DB::DBCurveDataIndices::IDName)
.Const("Base", (int)TS::DB::DBCurveDataIndices::Base)
.Const("Coefficient", (int)TS::DB::DBCurveDataIndices::Coefficient)
.Const("Index", (int)TS::DB::DBCurveDataIndices::Index)
.Const("Count", (int)TS::DB::DBCurveDataIndices::Count)
);

Sqrat::ConstTable()
.Enum("DBElementIndices", Sqrat::Enumeration()			//TS::DB::DBElementIndices
.Const("Parameter", (int)TS::DB::DBElementIndices::Parameter)
.Const("Attribute", (int)TS::DB::DBElementIndices::Attribute)
.Const("SkillType", (int)TS::DB::DBElementIndices::SkillType)
.Const("ItemType", (int)TS::DB::DBElementIndices::ItemType)
.Const("ItemSubType", (int)TS::DB::DBElementIndices::ItemSubType)
);

Sqrat::ConstTable()
.Enum("DBValueIndices", Sqrat::Enumeration()			//TS::DB::DBValueIndices
.Const("CommonFlag", (int)TS::DB::DBValueIndices::CommonFlag)
.Const("CommonInteger", (int)TS::DB::DBValueIndices::CommonInteger)
.Const("CommonString", (int)TS::DB::DBValueIndices::CommonString)
.Const("SelfActor", (int)TS::DB::DBValueIndices::SelfActor)
.Const("SelfClass", (int)TS::DB::DBValueIndices::SelfClass)
.Const("SelfUse", (int)TS::DB::DBValueIndices::SelfUse)
.Const("SelfState", (int)TS::DB::DBValueIndices::SelfState)
.Const("SelfUnit", (int)TS::DB::DBValueIndices::SelfUnit)
.Const("SelfEvent", (int)TS::DB::DBValueIndices::SelfEvent)
);

Sqrat::ConstTable()
.Enum("DBInitIndices", Sqrat::Enumeration()			//TS::DB::DBInitIndices
.Const("Party", (int)TS::DB::DBInitIndices::Party)
.Const("PartyButton", (int)TS::DB::DBInitIndices::PartyButton)
.Const("Item", (int)TS::DB::DBInitIndices::Item)
.Const("ItemButton", (int)TS::DB::DBInitIndices::ItemButton)
.Const("Money", (int)TS::DB::DBInitIndices::Money)
);

Sqrat::ConstTable()
.Enum("DBMaterialIndices", Sqrat::Enumeration()			//TS::DB::DBMaterialIndices
.Const("Face", (int)TS::DB::DBMaterialIndices::Face)
.Const("Graphics", (int)TS::DB::DBMaterialIndices::Graphics)
.Const("Sound", (int)TS::DB::DBMaterialIndices::Sound)
.Const("Font", (int)TS::DB::DBMaterialIndices::Font)
);

Sqrat::ConstTable()
.Enum("DBUnitEnemyIndices", Sqrat::Enumeration()			//TS::DB::DBUnitEnemyIndices
.Const("FixedID", (int)TS::DB::DBUnitEnemyIndices::FixedID)
.Const("Position", (int)TS::DB::DBUnitEnemyIndices::Position)
);

Sqrat::ConstTable()
.Enum("DBEffectColumnIndices", Sqrat::Enumeration()			//TS::DB::DBEffectColumnIndices
.Const("FrameLength", (int)TS::DB::DBEffectColumnIndices::FrameLength)
.Const("DrawDest", (int)TS::DB::DBEffectColumnIndices::DrawDest)
.Const("Layers", (int)TS::DB::DBEffectColumnIndices::Layers)
);

Sqrat::ConstTable()
.Enum("DBTilesetColumnIndices", Sqrat::Enumeration()			//TS::DB::DBTilesetColumnIndices
.Const("Comment", (int)TS::DB::DBTilesetColumnIndices::Comment)
.Const("AutoTiles", (int)TS::DB::DBTilesetColumnIndices::AutoTiles)
.Const("ObjTiles", (int)TS::DB::DBTilesetColumnIndices::ObjTiles)
);

Sqrat::ConstTable()
.Enum("DBEffectViewPosition", Sqrat::Enumeration()			//TS::DB::DBEffectViewPosition
.Const("Each", (int)TS::DB::DBEffectViewPosition::Each)
.Const("OnlyOne", (int)TS::DB::DBEffectViewPosition::OnlyOne)
.Const("CenterScreen", (int)TS::DB::DBEffectViewPosition::CenterScreen)
);

Sqrat::ConstTable()
.Enum("UseType", Sqrat::Enumeration()			//TS::DBLiteral::UseType
.Const("Script", (int)TS::DBLiteral::UseType::Script)
.Const("Physic", (int)TS::DBLiteral::UseType::Physic)
.Const("Magic", (int)TS::DBLiteral::UseType::Magic)
.Const("Heal", (int)TS::DBLiteral::UseType::Heal)
.Const("Utility", (int)TS::DBLiteral::UseType::Utility)
);

Sqrat::ConstTable()
.Enum("SceneLimit", Sqrat::Enumeration()			//TS::DBLiteral::SceneLimit
.Const("AlwaysNG", (int)TS::DBLiteral::SceneLimit::AlwaysNG)
.Const("OnlyMap", (int)TS::DBLiteral::SceneLimit::OnlyMap)
.Const("OnlyBattle", (int)TS::DBLiteral::SceneLimit::OnlyBattle)
.Const("AlwaysOK", (int)TS::DBLiteral::SceneLimit::AlwaysOK)
);

Sqrat::ConstTable()
.Enum("TargetType", Sqrat::Enumeration()			//TS::DBLiteral::TargetType
.Const("None", (int)TS::DBLiteral::TargetType::None)
.Const("OneParty", (int)TS::DBLiteral::TargetType::OneParty)
.Const("OnePartyWithDead", (int)TS::DBLiteral::TargetType::OnePartyWithDead)
.Const("AllParty", (int)TS::DBLiteral::TargetType::AllParty)
.Const("AllPartyWithDead", (int)TS::DBLiteral::TargetType::AllPartyWithDead)
.Const("OneEnemy", (int)TS::DBLiteral::TargetType::OneEnemy)
.Const("AllEnemy", (int)TS::DBLiteral::TargetType::AllEnemy)
.Const("Place", (int)TS::DBLiteral::TargetType::Place)
);

Sqrat::ConstTable()
.Enum("StateLimit", Sqrat::Enumeration()			//TS::DBLiteral::StateLimit
.Const("None", (int)TS::DBLiteral::StateLimit::None)
.Const("KO", (int)TS::DBLiteral::StateLimit::KO)
.Const("ForbiddenAction", (int)TS::DBLiteral::StateLimit::ForbiddenAction)
);

Sqrat::ConstTable()
.Enum("StateSolve", Sqrat::Enumeration()			//TS::DBLiteral::StateSolve
.Const("None", (int)TS::DBLiteral::StateSolve::None)
.Const("Damamge", (int)TS::DBLiteral::StateSolve::Damamge)
.Const("Turn", (int)TS::DBLiteral::StateSolve::Turn)
.Const("Walk", (int)TS::DBLiteral::StateSolve::Walk)
);

Sqrat::ConstTable()
.Enum("FontDrawing", Sqrat::Enumeration()			//TS::DBLiteral::FontDrawing
.Const("Normal", (int)TS::DBLiteral::FontDrawing::Normal)
.Const("Edge", (int)TS::DBLiteral::FontDrawing::Edge)
.Const("AS", (int)TS::DBLiteral::FontDrawing::AS)
.Const("EdgeAS", (int)TS::DBLiteral::FontDrawing::EdgeAS)
);

Sqrat::ConstTable()
.Enum("Addon", Sqrat::Enumeration()			//TS::DBLiteral::Addon
.Const("SelfValuePlus", (int)TS::DBLiteral::Addon::SelfValuePlus)
.Const("ParameterPlus", (int)TS::DBLiteral::Addon::ParameterPlus)
.Const("AttributeResistanceSet", (int)TS::DBLiteral::Addon::AttributeResistanceSet)
.Const("StateResistanceSet", (int)TS::DBLiteral::Addon::StateResistanceSet)
.Const("LockEquipType", (int)TS::DBLiteral::Addon::LockEquipType)
.Const("SetEquipType", (int)TS::DBLiteral::Addon::SetEquipType)
.Const("SetEquipSubType", (int)TS::DBLiteral::Addon::SetEquipSubType)
.Const("SetSkillUse", (int)TS::DBLiteral::Addon::SetSkillUse)
.Const("SetSkillType", (int)TS::DBLiteral::Addon::SetSkillType)
);

Sqrat::ConstTable().Const("Map_TileColumnCount", TS::Map::TileColumnCount);			//int

Sqrat::ConstTable().Const("Map_AutoTileRowLength", TS::Map::AutoTileRowLength);			//int

Sqrat::ConstTable().Const("Map_MapSizeMaxLength", TS::Map::MapSizeMaxLength);			//int

Sqrat::ConstTable()
.Enum("Layer", Sqrat::Enumeration()			//TS::Map::Layer
.Const("Low", (int)TS::Map::Layer::Low)
.Const("Middle", (int)TS::Map::Layer::Middle)
.Const("High", (int)TS::Map::Layer::High)
.Const("Shadow", (int)TS::Map::Layer::Shadow)
.Const("Event", (int)TS::Map::Layer::Event)
);

Sqrat::ConstTable()
.Enum("QuarterTile", Sqrat::Enumeration()			//TS::Map::QuarterTile
.Const("LeftTop", (int)TS::Map::QuarterTile::LeftTop)
.Const("RightTop", (int)TS::Map::QuarterTile::RightTop)
.Const("LeftBottom", (int)TS::Map::QuarterTile::LeftBottom)
.Const("RightBottom", (int)TS::Map::QuarterTile::RightBottom)
);

Sqrat::ConstTable()
.Enum("Direction4", Sqrat::Enumeration()			//TS::Map::Direction4
.Const("South", (int)TS::Map::Direction4::South)
.Const("West", (int)TS::Map::Direction4::West)
.Const("East", (int)TS::Map::Direction4::East)
.Const("North", (int)TS::Map::Direction4::North)
.Const("None", (int)TS::Map::Direction4::None)
);

Sqrat::ConstTable()
.Enum("Direction8", Sqrat::Enumeration()			//TS::Map::Direction8
.Const("SouthWest", (int)TS::Map::Direction8::SouthWest)
.Const("South", (int)TS::Map::Direction8::South)
.Const("SouthEast", (int)TS::Map::Direction8::SouthEast)
.Const("West", (int)TS::Map::Direction8::West)
.Const("None", (int)TS::Map::Direction8::None)
.Const("East", (int)TS::Map::Direction8::East)
.Const("NorthWest", (int)TS::Map::Direction8::NorthWest)
.Const("North", (int)TS::Map::Direction8::North)
.Const("NorthEast", (int)TS::Map::Direction8::NorthEast)
);

Sqrat::ConstTable()
.Enum("Speed", Sqrat::Enumeration()			//TS::Map::Speed
.Const("VeryLow", (int)TS::Map::Speed::VeryLow)
.Const("Low", (int)TS::Map::Speed::Low)
.Const("Normal", (int)TS::Map::Speed::Normal)
.Const("High", (int)TS::Map::Speed::High)
.Const("VeryHigh", (int)TS::Map::Speed::VeryHigh)
.Const("Moment", (int)TS::Map::Speed::Moment)
);

Sqrat::ConstTable()
.Enum("TileDataType", Sqrat::Enumeration()			//TS::Map::TileDataType
.Const("Hit", (int)TS::Map::TileDataType::Hit)
.Const("InOut", (int)TS::Map::TileDataType::InOut)
.Const("Priority", (int)TS::Map::TileDataType::Priority)
.Const("GroupNumber", (int)TS::Map::TileDataType::GroupNumber)
);

Sqrat::ConstTable()
.Enum("HitType", Sqrat::Enumeration()			//TS::Map::HitType
.Const("OK", (int)TS::Map::HitType::OK)
.Const("NG", (int)TS::Map::HitType::NG)
.Const("Depend", (int)TS::Map::HitType::Depend)
);

Sqrat::ConstTable()
.Enum("InOutType", Sqrat::Enumeration()			//TS::Map::InOutType
.Const("None", (int)TS::Map::InOutType::None)
.Const("OKBottom", (int)TS::Map::InOutType::OKBottom)
.Const("OKLeft", (int)TS::Map::InOutType::OKLeft)
.Const("OKRight", (int)TS::Map::InOutType::OKRight)
.Const("OKTop", (int)TS::Map::InOutType::OKTop)
);

Sqrat::ConstTable()
.Enum("TileDrawPriority", Sqrat::Enumeration()			//TS::Map::TileDrawPriority
.Const("Normal", (int)TS::Map::TileDrawPriority::Normal)
.Const("SemiTransparent", (int)TS::Map::TileDrawPriority::SemiTransparent)
.Const("SemiTopMost", (int)TS::Map::TileDrawPriority::SemiTopMost)
.Const("TopMost", (int)TS::Map::TileDrawPriority::TopMost)
);

Sqrat::ConstTable()
.Enum("AutoTilePattern", Sqrat::Enumeration()			//TS::Map::AutoTilePattern
.Const("Isolate", (int)TS::Map::AutoTilePattern::Isolate)
.Const("Vertical", (int)TS::Map::AutoTilePattern::Vertical)
.Const("Horizontal", (int)TS::Map::AutoTilePattern::Horizontal)
.Const("FourSide", (int)TS::Map::AutoTilePattern::FourSide)
.Const("AllSide", (int)TS::Map::AutoTilePattern::AllSide)
);

Sqrat::ConstTable()
.Enum("MapLoopType", Sqrat::Enumeration()			//TS::Map::MapLoopType
.Const("None", (int)TS::Map::MapLoopType::None)
.Const("Vertical", (int)TS::Map::MapLoopType::Vertical)
.Const("Horizontal", (int)TS::Map::MapLoopType::Horizontal)
.Const("Both", (int)TS::Map::MapLoopType::Both)
);

Sqrat::ConstTable()
.Enum("ScrollType", Sqrat::Enumeration()			//TS::Map::ScrollType
.Const("None", (int)TS::Map::ScrollType::None)
.Const("FollowChar", (int)TS::Map::ScrollType::FollowChar)
.Const("LeftDown", (int)TS::Map::ScrollType::LeftDown)
.Const("Down", (int)TS::Map::ScrollType::Down)
.Const("RightDown", (int)TS::Map::ScrollType::RightDown)
.Const("Left", (int)TS::Map::ScrollType::Left)
.Const("Right", (int)TS::Map::ScrollType::Right)
.Const("LeftUp", (int)TS::Map::ScrollType::LeftUp)
.Const("Up", (int)TS::Map::ScrollType::Up)
.Const("RightUp", (int)TS::Map::ScrollType::RightUp)
);

Sqrat::ConstTable()
.Enum("EventScriptType", Sqrat::Enumeration()			//TS::Map::EventScriptType
.Const("Page", (int)TS::Map::EventScriptType::Page)
.Const("Trigger", (int)TS::Map::EventScriptType::Trigger)
.Const("Move", (int)TS::Map::EventScriptType::Move)
.Const("Run", (int)TS::Map::EventScriptType::Run)
);

Sqrat::ConstTable()
.Enum("EventTriggerType", Sqrat::Enumeration()			//TS::Map::EventTriggerType
.Const("None", (int)TS::Map::EventTriggerType::None)
.Const("PressEnterKey", (int)TS::Map::EventTriggerType::PressEnterKey)
.Const("Push", (int)TS::Map::EventTriggerType::Push)
.Const("Pushed", (int)TS::Map::EventTriggerType::Pushed)
.Const("Auto", (int)TS::Map::EventTriggerType::Auto)
.Const("AfterMoveMap", (int)TS::Map::EventTriggerType::AfterMoveMap)
.Const("Parallel", (int)TS::Map::EventTriggerType::Parallel)
);

Sqrat::ConstTable()
.Enum("EventMoveType", Sqrat::Enumeration()			//TS::Map::EventMoveType
.Const("None", (int)TS::Map::EventMoveType::None)
.Const("Random", (int)TS::Map::EventMoveType::Random)
.Const("Near", (int)TS::Map::EventMoveType::Near)
.Const("Away", (int)TS::Map::EventMoveType::Away)
.Const("Script", (int)TS::Map::EventMoveType::Script)
);

Sqrat::ConstTable()
.Enum("EventDrawPriority", Sqrat::Enumeration()			//TS::Map::EventDrawPriority
.Const("Low", (int)TS::Map::EventDrawPriority::Low)
.Const("Middle", (int)TS::Map::EventDrawPriority::Middle)
.Const("High", (int)TS::Map::EventDrawPriority::High)
);

Sqrat::ConstTable()
.Enum("EventAnimation", Sqrat::Enumeration()			//TS::Map::EventAnimation
.Const("Move1", (int)TS::Map::EventAnimation::Move1)
.Const("Stop", (int)TS::Map::EventAnimation::Stop)
.Const("Move2", (int)TS::Map::EventAnimation::Move2)
.Const("Count", (int)TS::Map::EventAnimation::Count)
);

Sqrat::ConstTable()
.Enum("EventMoveTask", Sqrat::Enumeration()			//TS::Map::EventMoveTask
.Const("Direction", (int)TS::Map::EventMoveTask::Direction)
.Const("Forward", (int)TS::Map::EventMoveTask::Forward)
.Const("Backward", (int)TS::Map::EventMoveTask::Backward)
.Const("Move", (int)TS::Map::EventMoveTask::Move)
.Const("MoveRandom", (int)TS::Map::EventMoveTask::MoveRandom)
.Const("MoveToPlayer", (int)TS::Map::EventMoveTask::MoveToPlayer)
.Const("MoveAgainstPlayer", (int)TS::Map::EventMoveTask::MoveAgainstPlayer)
.Const("TurnLeft", (int)TS::Map::EventMoveTask::TurnLeft)
.Const("TurnRight", (int)TS::Map::EventMoveTask::TurnRight)
.Const("Turn180", (int)TS::Map::EventMoveTask::Turn180)
.Const("TurnRandom", (int)TS::Map::EventMoveTask::TurnRandom)
.Const("TurnToPlayer", (int)TS::Map::EventMoveTask::TurnToPlayer)
.Const("TurnAgainstPlayer", (int)TS::Map::EventMoveTask::TurnAgainstPlayer)
.Const("Wait", (int)TS::Map::EventMoveTask::Wait)
.Const("SetMoveSpeed", (int)TS::Map::EventMoveTask::SetMoveSpeed)
.Const("SetMoveFrequency", (int)TS::Map::EventMoveTask::SetMoveFrequency)
.Const("EnabledWalkAnim", (int)TS::Map::EventMoveTask::EnabledWalkAnim)
.Const("EnabledStopAnim", (int)TS::Map::EventMoveTask::EnabledStopAnim)
.Const("NoHit", (int)TS::Map::EventMoveTask::NoHit)
.Const("FixedDirection", (int)TS::Map::EventMoveTask::FixedDirection)
.Const("Hide", (int)TS::Map::EventMoveTask::Hide)
.Const("ChangeGraphics", (int)TS::Map::EventMoveTask::ChangeGraphics)
.Const("SQ", (int)TS::Map::EventMoveTask::SQ)
.Const("LockRoute", (int)TS::Map::EventMoveTask::LockRoute)
.Const("Repeat", (int)TS::Map::EventMoveTask::Repeat)
);

Sqrat::ConstTable().Const("System_FixedFPS", TS::System::FixedFPS);			//int

Sqrat::ConstTable()
.Enum("RunMode", Sqrat::Enumeration()			//TS::System::RunMode
.Const("MainGame", (int)TS::System::RunMode::MainGame)
.Const("MapTest", (int)TS::System::RunMode::MapTest)
.Const("BattleTest", (int)TS::System::RunMode::BattleTest)
.Const("SQCompile", (int)TS::System::RunMode::SQCompile)
.Const("SQTest", (int)TS::System::RunMode::SQTest)
.Const("DXArchive", (int)TS::System::RunMode::DXArchive)
);

Sqrat::ConstTable()
.Enum("ExitCode", Sqrat::Enumeration()			//TS::System::ExitCode
.Const("Error", (int)TS::System::ExitCode::Error)
.Const("Success", (int)TS::System::ExitCode::Success)
.Const("Question", (int)TS::System::ExitCode::Question)
.Const("InvalidSystemDB", (int)TS::System::ExitCode::InvalidSystemDB)
);

Sqrat::ConstTable()
.Enum("Scene", Sqrat::Enumeration()			//TS::System::Scene
.Const("All", (int)TS::System::Scene::All)
.Const("Title", (int)TS::System::Scene::Title)
.Const("Map", (int)TS::System::Scene::Map)
.Const("Menu", (int)TS::System::Scene::Menu)
.Const("Shop", (int)TS::System::Scene::Shop)
.Const("Battle", (int)TS::System::Scene::Battle)
);

Sqrat::ConstTable()
.Enum("MessageBoxIcon", Sqrat::Enumeration()			//TS::System::MessageBoxIcon
.Const("None", (int)TS::System::MessageBoxIcon::None)
.Const("Warning", (int)TS::System::MessageBoxIcon::Warning)
.Const("Information", (int)TS::System::MessageBoxIcon::Information)
.Const("Error", (int)TS::System::MessageBoxIcon::Error)
);

Sqrat::ConstTable()
.Enum("MessageBoxButton", Sqrat::Enumeration()			//TS::System::MessageBoxButton
.Const("OK", (int)TS::System::MessageBoxButton::OK)
.Const("OKCancel", (int)TS::System::MessageBoxButton::OKCancel)
.Const("YesNo", (int)TS::System::MessageBoxButton::YesNo)
.Const("YesNoCancel", (int)TS::System::MessageBoxButton::YesNoCancel)
.Const("RetryCancel", (int)TS::System::MessageBoxButton::RetryCancel)
.Const("AbortRetryIgnore", (int)TS::System::MessageBoxButton::AbortRetryIgnore)
);

Sqrat::ConstTable()
.Enum("MessageBoxResult", Sqrat::Enumeration()			//TS::System::MessageBoxResult
.Const("OK", (int)TS::System::MessageBoxResult::OK)
.Const("Cancel", (int)TS::System::MessageBoxResult::Cancel)
.Const("Yes", (int)TS::System::MessageBoxResult::Yes)
.Const("No", (int)TS::System::MessageBoxResult::No)
.Const("Abort", (int)TS::System::MessageBoxResult::Abort)
.Const("Retry", (int)TS::System::MessageBoxResult::Retry)
.Const("Ignore", (int)TS::System::MessageBoxResult::Ignore)
);

Sqrat::ConstTable()
.Enum("ErrorID", Sqrat::Enumeration()			//TS::System::ErrorID
.Const("NotFound", (int)TS::System::ErrorID::NotFound)
.Const("Exists", (int)TS::System::ErrorID::Exists)
.Const("OutOfRange", (int)TS::System::ErrorID::OutOfRange)
.Const("Except", (int)TS::System::ErrorID::Except)
.Const("Invalid", (int)TS::System::ErrorID::Invalid)
.Const("Failed", (int)TS::System::ErrorID::Failed)
.Const("WinAPI", (int)TS::System::ErrorID::WinAPI)
.Const("SQ", (int)TS::System::ErrorID::SQ)
);

Sqrat::ConstTable()
.Enum("KeyID", Sqrat::Enumeration()			//TS::System::KeyID
.Const("Backspace", (int)TS::System::KeyID::Backspace)
.Const("Tab", (int)TS::System::KeyID::Tab)
.Const("Enter", (int)TS::System::KeyID::Enter)
.Const("LShift", (int)TS::System::KeyID::LShift)
.Const("RShift", (int)TS::System::KeyID::RShift)
.Const("LCtrl", (int)TS::System::KeyID::LCtrl)
.Const("RCtrl", (int)TS::System::KeyID::RCtrl)
.Const("Escape", (int)TS::System::KeyID::Escape)
.Const("Space", (int)TS::System::KeyID::Space)
.Const("PageUp", (int)TS::System::KeyID::PageUp)
.Const("PageDown", (int)TS::System::KeyID::PageDown)
.Const("End", (int)TS::System::KeyID::End)
.Const("Home", (int)TS::System::KeyID::Home)
.Const("Left", (int)TS::System::KeyID::Left)
.Const("Up", (int)TS::System::KeyID::Up)
.Const("Right", (int)TS::System::KeyID::Right)
.Const("Down", (int)TS::System::KeyID::Down)
.Const("Insert", (int)TS::System::KeyID::Insert)
.Const("Delete", (int)TS::System::KeyID::Delete)
.Const("Minus", (int)TS::System::KeyID::Minus)
.Const("Yen", (int)TS::System::KeyID::Yen)
.Const("Prevtrack", (int)TS::System::KeyID::Prevtrack)
.Const("Period", (int)TS::System::KeyID::Period)
.Const("Slash", (int)TS::System::KeyID::Slash)
.Const("LAlt", (int)TS::System::KeyID::LAlt)
.Const("RAlt", (int)TS::System::KeyID::RAlt)
.Const("ScrollLock", (int)TS::System::KeyID::ScrollLock)
.Const("SemiColon", (int)TS::System::KeyID::SemiColon)
.Const("Colon", (int)TS::System::KeyID::Colon)
.Const("LBracket", (int)TS::System::KeyID::LBracket)
.Const("RBracket", (int)TS::System::KeyID::RBracket)
.Const("At", (int)TS::System::KeyID::At)
.Const("BackSlash", (int)TS::System::KeyID::BackSlash)
.Const("Comma", (int)TS::System::KeyID::Comma)
.Const("Kanji", (int)TS::System::KeyID::Kanji)
.Const("Convert", (int)TS::System::KeyID::Convert)
.Const("NoConvert", (int)TS::System::KeyID::NoConvert)
.Const("Kana", (int)TS::System::KeyID::Kana)
.Const("Apps", (int)TS::System::KeyID::Apps)
.Const("CapsLock", (int)TS::System::KeyID::CapsLock)
.Const("PrintScreen", (int)TS::System::KeyID::PrintScreen)
.Const("PauseBreak", (int)TS::System::KeyID::PauseBreak)
.Const("LWin", (int)TS::System::KeyID::LWin)
.Const("RWin", (int)TS::System::KeyID::RWin)
.Const("NumLock", (int)TS::System::KeyID::NumLock)
.Const("Num0", (int)TS::System::KeyID::Num0)
.Const("Num1", (int)TS::System::KeyID::Num1)
.Const("Num2", (int)TS::System::KeyID::Num2)
.Const("Num3", (int)TS::System::KeyID::Num3)
.Const("Num4", (int)TS::System::KeyID::Num4)
.Const("Num5", (int)TS::System::KeyID::Num5)
.Const("Num6", (int)TS::System::KeyID::Num6)
.Const("Num7", (int)TS::System::KeyID::Num7)
.Const("Num8", (int)TS::System::KeyID::Num8)
.Const("Num9", (int)TS::System::KeyID::Num9)
.Const("NumMultiply", (int)TS::System::KeyID::NumMultiply)
.Const("NumAdd", (int)TS::System::KeyID::NumAdd)
.Const("NumSubtract", (int)TS::System::KeyID::NumSubtract)
.Const("NumDecimal", (int)TS::System::KeyID::NumDecimal)
.Const("NumDivide", (int)TS::System::KeyID::NumDivide)
.Const("NumEnter", (int)TS::System::KeyID::NumEnter)
.Const("F1", (int)TS::System::KeyID::F1)
.Const("F2", (int)TS::System::KeyID::F2)
.Const("F3", (int)TS::System::KeyID::F3)
.Const("F4", (int)TS::System::KeyID::F4)
.Const("F5", (int)TS::System::KeyID::F5)
.Const("F6", (int)TS::System::KeyID::F6)
.Const("F7", (int)TS::System::KeyID::F7)
.Const("F8", (int)TS::System::KeyID::F8)
.Const("F9", (int)TS::System::KeyID::F9)
.Const("F10", (int)TS::System::KeyID::F10)
.Const("F11", (int)TS::System::KeyID::F11)
.Const("F12", (int)TS::System::KeyID::F12)
.Const("A", (int)TS::System::KeyID::A)
.Const("B", (int)TS::System::KeyID::B)
.Const("C", (int)TS::System::KeyID::C)
.Const("D", (int)TS::System::KeyID::D)
.Const("E", (int)TS::System::KeyID::E)
.Const("F", (int)TS::System::KeyID::F)
.Const("G", (int)TS::System::KeyID::G)
.Const("H", (int)TS::System::KeyID::H)
.Const("I", (int)TS::System::KeyID::I)
.Const("J", (int)TS::System::KeyID::J)
.Const("K", (int)TS::System::KeyID::K)
.Const("L", (int)TS::System::KeyID::L)
.Const("M", (int)TS::System::KeyID::M)
.Const("N", (int)TS::System::KeyID::N)
.Const("O", (int)TS::System::KeyID::O)
.Const("P", (int)TS::System::KeyID::P)
.Const("Q", (int)TS::System::KeyID::Q)
.Const("R", (int)TS::System::KeyID::R)
.Const("S", (int)TS::System::KeyID::S)
.Const("T", (int)TS::System::KeyID::T)
.Const("U", (int)TS::System::KeyID::U)
.Const("V", (int)TS::System::KeyID::V)
.Const("W", (int)TS::System::KeyID::W)
.Const("X", (int)TS::System::KeyID::X)
.Const("Y", (int)TS::System::KeyID::Y)
.Const("Z", (int)TS::System::KeyID::Z)
.Const("Key0", (int)TS::System::KeyID::Key0)
.Const("Key1", (int)TS::System::KeyID::Key1)
.Const("Key2", (int)TS::System::KeyID::Key2)
.Const("Key3", (int)TS::System::KeyID::Key3)
.Const("Key4", (int)TS::System::KeyID::Key4)
.Const("Key5", (int)TS::System::KeyID::Key5)
.Const("Key6", (int)TS::System::KeyID::Key6)
.Const("Key7", (int)TS::System::KeyID::Key7)
.Const("Key8", (int)TS::System::KeyID::Key8)
.Const("Key9", (int)TS::System::KeyID::Key9)
);

Sqrat::ConstTable()
.Enum("PadID", Sqrat::Enumeration()			//TS::System::PadID
.Const("Down", (int)TS::System::PadID::Down)
.Const("Left", (int)TS::System::PadID::Left)
.Const("Right", (int)TS::System::PadID::Right)
.Const("Up", (int)TS::System::PadID::Up)
.Const("Button1", (int)TS::System::PadID::Button1)
.Const("Button2", (int)TS::System::PadID::Button2)
.Const("Button3", (int)TS::System::PadID::Button3)
.Const("Button4", (int)TS::System::PadID::Button4)
.Const("Button5", (int)TS::System::PadID::Button5)
.Const("Button6", (int)TS::System::PadID::Button6)
.Const("Button7", (int)TS::System::PadID::Button7)
.Const("Button8", (int)TS::System::PadID::Button8)
.Const("Button9", (int)TS::System::PadID::Button9)
.Const("Button10", (int)TS::System::PadID::Button10)
);

Sqrat::ConstTable()
.Enum("MouseID", Sqrat::Enumeration()			//TS::System::MouseID
.Const("Left", (int)TS::System::MouseID::Left)
.Const("Right", (int)TS::System::MouseID::Right)
.Const("Middle", (int)TS::System::MouseID::Middle)
);

Sqrat::ConstTable()
.Enum("TextAlign", Sqrat::Enumeration()			//TS::Sprite::TextAlign
.Const("Left", (int)TS::Sprite::TextAlign::Left)
.Const("Center", (int)TS::Sprite::TextAlign::Center)
.Const("Right", (int)TS::Sprite::TextAlign::Right)
);

Sqrat::ConstTable()
.Enum("WipeState", Sqrat::Enumeration()			//TS::Sprite::WipeState
.Const("None", (int)TS::Sprite::WipeState::None)
.Const("WipeIn", (int)TS::Sprite::WipeState::WipeIn)
.Const("WipeOut", (int)TS::Sprite::WipeState::WipeOut)
);

Sqrat::ConstTable()
.Enum("MaterialType", Sqrat::Enumeration()			//TS::Material::MaterialType
.Const("Graphics", (int)TS::Material::MaterialType::Graphics)
.Const("Sound", (int)TS::Material::MaterialType::Sound)
.Const("Font", (int)TS::Material::MaterialType::Font)
);

Sqrat::ConstTable().Const("Material_FontNormalSize", TS::Material::FontNormalSize);			//int

Sqrat::ConstTable().Const("Material_FontNormalThick", TS::Material::FontNormalThick);			//int

Sqrat::ConstTable().Const("Material_FontNormalLineSpace", TS::Material::FontNormalLineSpace);			//int

Sqrat::ConstTable().Const("Material_FontNormalType", TS::Material::FontNormalType);			//int

Sqrat::ConstTable().Const("Material_Common_MinPitch", TS::Material::Common_MinPitch);			//int

Sqrat::ConstTable().Const("Material_Common_MaxPitch", TS::Material::Common_MaxPitch);			//int

Sqrat::ConstTable().Const("Material_MIDI_MinVolume", TS::Material::MIDI_MinVolume);			//int

Sqrat::ConstTable().Const("Material_MIDI_MaxVolume", TS::Material::MIDI_MaxVolume);			//int

Sqrat::ConstTable().Const("Material_MIDI_MinPitch", TS::Material::MIDI_MinPitch);			//int

Sqrat::ConstTable().Const("Material_MIDI_MaxPitch", TS::Material::MIDI_MaxPitch);			//int

Sqrat::ConstTable().Const("Material_MinVolume", TS::Material::MinVolume);			//int

Sqrat::ConstTable().Const("Material_MaxVolume", TS::Material::MaxVolume);			//int

Sqrat::ConstTable().Const("Material_MinPitch", TS::Material::MinPitch);			//int

Sqrat::ConstTable().Const("Material_MaxPitch", TS::Material::MaxPitch);			//int

Sqrat::ConstTable().Const("Material_MinPan", TS::Material::MinPan);			//int

Sqrat::ConstTable().Const("Material_MaxPan", TS::Material::MaxPan);			//int

Sqrat::ConstTable()
.Enum("PlayStatus", Sqrat::Enumeration()			//TS::Material::PlayStatus
.Const("Error", (int)TS::Material::PlayStatus::Error)
.Const("Stop", (int)TS::Material::PlayStatus::Stop)
.Const("Playing", (int)TS::Material::PlayStatus::Playing)
);

Sqrat::ConstTable()
.Enum("PlayType", Sqrat::Enumeration()			//TS::Material::PlayType
.Const("Wait", (int)TS::Material::PlayType::Wait)
.Const("Back", (int)TS::Material::PlayType::Back)
.Const("Loop", (int)TS::Material::PlayType::Loop)
);

Sqrat::ConstTable()
.Enum("DBType", Sqrat::Enumeration()			//TS::DB::DBType
.Const("FDB", (int)TS::DB::DBType::FDB)
.Const("VDB", (int)TS::DB::DBType::VDB)
.Const("UDB", (int)TS::DB::DBType::UDB)
);

Sqrat::ConstTable()
.Enum("ColumnType", Sqrat::Enumeration()			//TS::DB::ColumnType
.Const("None", (int)TS::DB::ColumnType::None)
.Const("OneNumber", (int)TS::DB::ColumnType::OneNumber)
.Const("Numbers", (int)TS::DB::ColumnType::Numbers)
.Const("String", (int)TS::DB::ColumnType::String)
.Const("Boolean", (int)TS::DB::ColumnType::Boolean)
.Const("IDNames", (int)TS::DB::ColumnType::IDNames)
.Const("IDValues", (int)TS::DB::ColumnType::IDValues)
);

Sqrat::ConstTable()
.Enum("FDBIndices", Sqrat::Enumeration()			//TS::DB::FDBIndices
.Const("System", (int)TS::DB::FDBIndices::System)
.Const("Material", (int)TS::DB::FDBIndices::Material)
.Const("Element", (int)TS::DB::FDBIndices::Element)
.Const("Class", (int)TS::DB::FDBIndices::Class)
.Const("Skill", (int)TS::DB::FDBIndices::Skill)
.Const("Item", (int)TS::DB::FDBIndices::Item)
.Const("State", (int)TS::DB::FDBIndices::State)
.Const("Char", (int)TS::DB::FDBIndices::Char)
.Const("Enemy", (int)TS::DB::FDBIndices::Enemy)
.Const("Unit", (int)TS::DB::FDBIndices::Unit)
.Const("Effect", (int)TS::DB::FDBIndices::Effect)
.Const("Tileset", (int)TS::DB::FDBIndices::Tileset)
.Const("Init", (int)TS::DB::FDBIndices::Init)
);

Sqrat::ConstTable()
.Enum("VDBIndices", Sqrat::Enumeration()			//TS::DB::VDBIndices
.Const("Value", (int)TS::DB::VDBIndices::Value)
.Const("Party", (int)TS::DB::VDBIndices::Party)
.Const("Actor", (int)TS::DB::VDBIndices::Actor)
);

Sqrat::ConstTable()
.Enum("AddonFrom", Sqrat::Enumeration()			//TS::DB::AddonFrom
.Const("Actor", (int)TS::DB::AddonFrom::Actor)
.Const("Class", (int)TS::DB::AddonFrom::Class)
.Const("Item", (int)TS::DB::AddonFrom::Item)
.Const("State", (int)TS::DB::AddonFrom::State)
);

Sqrat::ConstTable().Const("Map_MapLayerCountWithoutEV", TS::Map::MapLayerCountWithoutEV);			//int

Sqrat::ConstTable()
.Enum("SquareSpeed", Sqrat::Enumeration()			//TS::Map::SquareSpeed
.Const("VeryLow", (int)TS::Map::SquareSpeed::VeryLow)
.Const("Low", (int)TS::Map::SquareSpeed::Low)
.Const("Normal", (int)TS::Map::SquareSpeed::Normal)
.Const("High", (int)TS::Map::SquareSpeed::High)
.Const("VeryHigh", (int)TS::Map::SquareSpeed::VeryHigh)
.Const("Moment", (int)TS::Map::SquareSpeed::Moment)
);

Sqrat::ConstTable().Const("DB_ColumnNameMaterialHandle", TS::DB::ColumnNameMaterialHandle.c_str());			//string

Sqrat::ConstTable().Const("DB_System_GameTitle", TS::DB::System::GameTitle.c_str());			//string

Sqrat::ConstTable().Const("DB_System_GameVersion", TS::DB::System::GameVersion.c_str());			//string

Sqrat::ConstTable().Const("DB_System_WindowSize", TS::DB::System::WindowSize.c_str());			//string

Sqrat::ConstTable().Const("DB_System_AllowFullScreen", TS::DB::System::AllowFullScreen.c_str());			//string

Sqrat::ConstTable().Const("DB_System_IconFileName", TS::DB::System::IconFileName.c_str());			//string

Sqrat::ConstTable().Const("DB_System_NowLoadingFileName", TS::DB::System::NowLoadingFileName.c_str());			//string

Sqrat::ConstTable().Const("DB_System_StandardTransparentColor", TS::DB::System::StandardTransparentColor.c_str());			//string

Sqrat::ConstTable().Const("DB_System_DrawInterpolation", TS::DB::System::DrawInterpolation.c_str());			//string

Sqrat::ConstTable().Const("DB_System_EffectAsyncLoadMode", TS::DB::System::EffectAsyncLoadMode.c_str());			//string

Sqrat::ConstTable().Const("DB_System_EffectFPS", TS::DB::System::EffectFPS.c_str());			//string

Sqrat::ConstTable().Const("DB_System_TileSize", TS::DB::System::TileSize.c_str());			//string

Sqrat::ConstTable().Const("DB_System_IconSize", TS::DB::System::IconSize.c_str());			//string

Sqrat::ConstTable().Const("DB_System_MaxPartyCount", TS::DB::System::MaxPartyCount.c_str());			//string

Sqrat::ConstTable().Const("DB_System_MaxLevel", TS::DB::System::MaxLevel.c_str());			//string

Sqrat::ConstTable().Const("DB_System_MaxSaveDataCount", TS::DB::System::MaxSaveDataCount.c_str());			//string

Sqrat::ConstTable().Const("DB_System_ShadowTransparent", TS::DB::System::ShadowTransparent.c_str());			//string

Sqrat::ConstTable().Const("DB_System_FogTransparent", TS::DB::System::FogTransparent.c_str());			//string

Sqrat::ConstTable().Const("DB_System_EnabledSystemResource", TS::DB::System::EnabledSystemResource.c_str());			//string

Sqrat::ConstTable().Const("DB_System_EnabledLimitedDebug", TS::DB::System::EnabledLimitedDebug.c_str());			//string

Sqrat::RootTable().Bind("CSaveLoad", Sqrat::Class<CSaveLoad>()
.Func("GetAllSaveHeader", (&CSaveLoad::GetAllSaveHeader), 0, NULL)			//void|
.Func("CopyData", (&CSaveLoad::CopyData), 3, ".nn")			//bool|int fromIndex,int toIndex
.Func("DeleteData", (&CSaveLoad::DeleteData), 2, ".n")			//bool|int targetIndex
.Func("LoadData", (&CSaveLoad::LoadData), 2, ".n")			//bool|int targetIndex
.Func("SaveData", (&CSaveLoad::SaveData), 2, ".n")			//bool|int targetIndex
.Func("QuickSave", (&CSaveLoad::QuickSave), 0, NULL)			//bool|
.Func("QuickLoad", (&CSaveLoad::QuickLoad), 0, NULL)			//bool|
.Func("IsEnabled", (&CSaveLoad::IsEnabled), 2, ".n")			//bool|int index
.Func("GetSaveHeader", (&CSaveLoad::GetSaveHeader), 2, ".n")			//string|int index
.Func("GetSaveDateTime", (&CSaveLoad::GetSaveDateTime), 2, ".n")			//string|int index
);

Sqrat::RootTable().Bind("CSpriteGraphicArea", Sqrat::DerivedClass<CSprite::CSpriteData::CSpriteGraphicArea, CIDNameData>()
.Prop("VisibleRange", &CSprite::CSpriteData::CSpriteGraphicArea::getVisibleRange)			//CRectangle
.Prop("Location", &CSprite::CSpriteData::CSpriteGraphicArea::getLocation)			//CPoint
.Prop("Size", &CSprite::CSpriteData::CSpriteGraphicArea::getSize)			//CSize
.Var("ExtendRateV", &CSprite::CSpriteData::CSpriteGraphicArea::ExtendRateV)			//int
.Var("ExtendRateH", &CSprite::CSpriteData::CSpriteGraphicArea::ExtendRateH)			//int
.Func("Draw", (&CSprite::CSpriteData::CSpriteGraphicArea::Draw), 2, ".x")			//void|CPoint pos
.Func("InitVisibleRange", (&CSprite::CSpriteData::CSpriteGraphicArea::InitVisibleRange), 0, NULL)			//void|
);

Sqrat::RootTable().Bind("CSpriteTextArea", Sqrat::DerivedClass<CSprite::CSpriteData::CSpriteTextArea, CIDNameData>()
.Var("Text", &CSprite::CSpriteData::CSpriteTextArea::Text)			//string
.Var("Align", &CSprite::CSpriteData::CSpriteTextArea::Align)			//int
.Var("Color", &CSprite::CSpriteData::CSpriteTextArea::Color)			//int
.Var("FontID", &CSprite::CSpriteData::CSpriteTextArea::FontID)			//int
.Prop("DrawArea", &CSprite::CSpriteData::CSpriteTextArea::getDrawArea)			//CRectangle
.Prop("Location", &CSprite::CSpriteData::CSpriteTextArea::getLocation)			//CPoint
.Func("Draw", (&CSprite::CSpriteData::CSpriteTextArea::Draw), 2, ".x")			//void|CPoint pos
);

Sqrat::RootTable().Bind("CSpriteData", Sqrat::DerivedClass<CSprite::CSpriteData, CIDNameData>()
.Prop("WipeType", &CSprite::CSpriteData::getWipeType)			//int
.Prop("WipeSpeed", &CSprite::CSpriteData::getWipeSpeed)			//int
.Prop("WipeCounter", &CSprite::CSpriteData::getWipeCounter, &CSprite::CSpriteData::setWipeCounter)			//int
.Var("Hide", &CSprite::CSpriteData::Hide)			//bool
.Prop("Location", &CSprite::CSpriteData::getLocation)			//CPoint
.Prop("Trans", &CSprite::CSpriteData::getTrans, &CSprite::CSpriteData::setTrans)			//int
.Func("GetGraphicsCount", (&CSprite::CSpriteData::GetGraphicsCount), 0, NULL)			//int|
.Func("AddGraphics", (&CSprite::CSpriteData::AddGraphics), 5, ".snnn")			//int|string path,int x,int y,int baseHandle = -1
.Func("ChangeGraphics", (&CSprite::CSpriteData::ChangeGraphics), 6, ".nsnnn")			//void|int index,string path,int x,int y,int baseHandle = -1
.Func("DeleteGraphics", (&CSprite::CSpriteData::DeleteGraphics), 2, ".n")			//void|int index
.Func("GetGraphics", (&CSprite::CSpriteData::GetGraphics), 2, ".n")			//CSpriteGraphicArea*|int index
.Func("GetTextsCount", (&CSprite::CSpriteData::GetTextsCount), 0, NULL)			//int|
.Func("AddText", (&CSprite::CSpriteData::AddText), 7, ".snnnnn")			//int|string text,int x,int y,int color,int fontID,int align = static_cast<int>(TS::Sprite::TextAlign::Left)
.Func("ChangeText", (&CSprite::CSpriteData::ChangeText), 3, ".ns")			//void|int index,string text
.Func("DeleteText", (&CSprite::CSpriteData::DeleteText), 2, ".n")			//void|int index
.Func("GetText", (&CSprite::CSpriteData::GetText), 2, ".n")			//CSpriteTextArea*|int index
.Func("Draw", (&CSprite::CSpriteData::Draw), 0, NULL)			//void|
.Func("WipeIn", (&CSprite::CSpriteData::WipeIn), 3, ".nn")			//void|int type,int wipeSpeed
.Func("WipeOut", (&CSprite::CSpriteData::WipeOut), 4, ".nnb|i")			//void|int type,int wipeSpeed,bool afterDelete
);

Sqrat::RootTable().Bind("CSprite", Sqrat::Class<CSprite>()
.Var("ForbidLayerDraw", &CSprite::ForbidLayerDraw)			//bool
.Func("Add", (&CSprite::Add), 2, ".n")			//void|int ID
.Func("Delete", (&CSprite::Delete), 2, ".n")			//void|int ID
.Func("Clear", (&CSprite::Clear), 3, ".nn")			//void|int wipeType,int wipeSpeed = static_cast<int>(TS::Map::Speed::Normal)
.Func("Get", (&CSprite::Get), 2, ".n")			//CSpriteData*|int ID
.Func("AddNewGraphics", (&CSprite::AddNewGraphics), 5, ".nsnn")			//void|int ID,string path,int x,int y
.Func("AddLoadedGraphics", (&CSprite::AddLoadedGraphics), 5, ".nnnn")			//void|int ID,int graphID,int x,int y
.Func("AddWipeNewGraphics", (&CSprite::AddWipeNewGraphics), 7, ".nsnnnn")			//void|int ID,string path,int x,int y,int wipeType,int wipeSpeed
.Func("AddWipeLoadedGraphics", (&CSprite::AddWipeLoadedGraphics), 7, ".nnnnnn")			//void|int ID,int graphID,int x,int y,int wipeType,int wipeSpeed
.Func("Copy", (&CSprite::Copy), 5, ".nnnn")			//void|int newID,int baseID,int x,int y
.Func("CopyWipe", (&CSprite::CopyWipe), 7, ".nnnnnn")			//void|int newID,int baseID,int x,int y,int wipeType,int wipeSpeed
.Func("WipeInRange", (&CSprite::WipeInRange), 5, ".nnnn")			//void|int startID,int endID,int wipeType,int wipeSpeed
.Func("WipeOutRange", (&CSprite::WipeOutRange), 6, ".nnnnb|i")			//void|int startID,int endID,int wipeType,int wipeSpeed,bool afterDelete = true
);

Sqrat::RootTable().Bind("CGameObject", Sqrat::Class<CGameObject>()
.Prop("MT", &CGameObject::getMT)			//CMultiThread
.Prop("FPSKeeper", &CGameObject::getFPSKeeper)			//CFPS
.Prop("DXLib", &CGameObject::getDXLib)			//CDXLibWrapper
.Prop("DB", &CGameObject::getDB)			//CDatabase
.Prop("Sprite", &CGameObject::getSprite)			//CSprite
.Prop("SaveLoad", &CGameObject::getSaveLoad)			//CSaveLoad
.Prop("Material", &CGameObject::getMaterial)			//CMaterial
.Var("ForbidSceneDraw", &CGameObject::ForbidSceneDraw)			//bool
.Var("ForbidSceneInput", &CGameObject::ForbidSceneInput)			//bool
.Var("BGMKeep", &CGameObject::BGMKeep)			//bool
.Var("BGSKeep", &CGameObject::BGSKeep)			//bool
.Prop("CurrentPath", &CGameObject::getCurrentPath)			//string
.Prop("RunMode", &CGameObject::getRunMode)			//int
.Prop("WindowSize", &CGameObject::getWindowSize)			//CSize
.Prop("DrawBright", &CGameObject::getDrawBright)			//CRGBColor
.Func("Wait", (&CGameObject::Wait), 4, ".nb|ib|i")			//bool|double timeSecond,bool enabledMT,bool keyUpBreak
.Func("WaitKey", (&CGameObject::WaitKey), 2, ".b|i")			//void|bool enabledMT
.Func("IsAnyKeyDown", (&CGameObject::IsAnyKeyDown), 0, NULL)			//bool|
.Func("GetKeyInputCounter", (&CGameObject::GetKeyInputCounter), 2, ".n")			//int|int keyID
.Func("SetKeyInputCounter", (&CGameObject::SetKeyInputCounter), 3, ".nn")			//void|int keyID,int val
.Func("PrepareTransition", (&CGameObject::PrepareTransition), 0, NULL)			//void|
.Func("DoTransition", (&CGameObject::DoTransition), 3, ".sn")			//void|string blendFileName,int speed
.Func("FadeOut", (&CGameObject::FadeOut), 2, ".n")			//void|int speed
.Func("FadeIn", (&CGameObject::FadeIn), 2, ".n")			//void|int speed
.Func("StartSubScene", (&CGameObject::StartSubScene), 4, ".nb|ib|i")			//void|int sceneID,bool keepMainScene,bool waitMode
.Func("EndSubScene", (&CGameObject::EndSubScene), 0, NULL)			//void|
.Func("IsSubScene", (&CGameObject::IsSubScene), 0, NULL)			//bool|
.Func("ChangeToTitleScene", (&CGameObject::ChangeToTitleScene), 0, NULL)			//void|
.Func("ChangeToMapScene", (&CGameObject::ChangeToMapScene), 0, NULL)			//void|
.Func("GetCurrentMainSceneID", (&CGameObject::GetCurrentMainSceneID), 0, NULL)			//int|
.Func("GetCurrentSubSceneID", (&CGameObject::GetCurrentSubSceneID), 0, NULL)			//int|
.Func("CheckCurrentSceneID", (&CGameObject::CheckCurrentSceneID), 2, ".n")			//bool|int sceneID
.Func("DoSceneDraw", (&CGameObject::DoSceneDraw), 0, NULL)			//void|
.Func("DoSceneInput", (&CGameObject::DoSceneInput), 0, NULL)			//void|
.Prop("SceneMap", &CGameObject::getSceneMap)			//CSceneMap
.Prop("SceneTitle", &CGameObject::getSceneTitle)			//CSceneTitle
.Func("GetCurrentSceneName", (&CGameObject::GetCurrentSceneName), 0, NULL)			//string|
.Func("SQInclude", (&CGameObject::SQInclude), 2, ".s")			//void|string fileName
.Func("SQBreakPoint", (&CGameObject::SQBreakPoint), 0, NULL)			//void|
.Func("SQBreakResume", (&CGameObject::SQBreakResume), 0, NULL)			//void|
.StaticFunc("MsgBox", (&CGameObject::MsgBox), 5, ".ssnn")			//int|string msg,string caption,int iconID,int buttonID
.StaticFunc("ScanConsole", (&CGameObject::ScanConsole), 0, NULL)			//string|
.StaticFunc("PrintLog", ((void(*)(string))&CGameObject::PrintLog), 2, ".s")			//void|string text
.StaticFunc("ApplicationExit", (&CGameObject::ApplicationExit), 2, ".n")			//void|int code = static_cast<int>(TS::System::ExitCode::Error)
.StaticFunc("StartMeasureProcTime", (&CGameObject::StartMeasureProcTime), 0, NULL)			//void|
.StaticFunc("ContinueMeasureProcTime", (&CGameObject::ContinueMeasureProcTime), 2, ".s")			//void|string tag
.StaticFunc("EndMeasureProcTime", (&CGameObject::EndMeasureProcTime), 2, ".s")			//void|string tag
.StaticFunc("WithIn", (&CGameObject::WithIn), 4, ".nnn")			//int|int val,int _min,int _max
.StaticFunc("UpAround", (&CGameObject::UpAround), 4, ".nnn")			//int|int val,int _min,int _max
.StaticFunc("DownAround", (&CGameObject::DownAround), 4, ".nnn")			//int|int val,int _min,int _max
.StaticFunc("IntToBool", (&CGameObject::IntToBool), 2, ".n")			//bool|int value
.StaticFunc("BoolToInt", (&CGameObject::BoolToInt), 2, ".b|i")			//int|bool value
.StaticFunc("NextRand", (&CGameObject::NextRand), 3, ".nn")			//int|int min,int max
.StaticFunc("CutDQ", (&CGameObject::CutDQ), 2, ".s")			//string|string baseString
.StaticFunc("IntToAlphabet", (&CGameObject::IntToAlphabet), 2, ".n")			//string|int value
.StaticFunc("SubString", (&CGameObject::SubString), 4, ".snn")			//string|string src,int start,int length
.StaticFunc("Replace", (&CGameObject::Replace), 4, ".sss")			//string|string baseString,string before,string after
.StaticFunc("Insert", (&CGameObject::Insert), 4, ".sns")			//string|string dest,int index,string text
.StaticFunc("GetStringLength", (&CGameObject::GetStringLength), 2, ".s")			//int|string src
);

Sqrat::RootTable().SetInstance("Game", &Game);			//CGameObject
}
