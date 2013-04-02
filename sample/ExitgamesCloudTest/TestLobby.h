//
//  TestLobby.h
//  ExitgamesCloudTest
//
//  Created by Kim Jazz on 13. 2. 11..
//  Copyright (c) 2013년 Kim Jazz. All rights reserved.
//

#import "CMMHeader.h"
#import "SimplePopup.h"
#import "JGPhotonCloudClient.h"

#define varTestLobby_staticKey @"_TestLobby_"

#define varTestLobby_roomProperties_roomTitle @"rTit"

@interface TestLobby : CMMLayer<JGPhotonCloudLobbyDelegate>{
	CMMScrollMenuV *viewRoomList;
	CMMMenuItemL *btnCreate,*btnRefresh;
}

@end
