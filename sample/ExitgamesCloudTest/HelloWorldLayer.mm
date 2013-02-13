//
//  HelloWorldLayer.mm
//  ExitgamesCloudTest
//
//  Created by Kim Jazz on 13. 2. 12..
//  Copyright (c) 2013년 Kim Jazz. All rights reserved.
//

#import "HelloWorldLayer.h"
#import "TestStarter.h"
#import "TestLobby.h"

@implementation HelloWorldLayer

-(void)sceneDidEndTransition:(CMMScene *)scene_{
	CMMScene *sharedScene_ = [CMMScene sharedScene];
	
	[sharedScene_ addStaticLayerItemWithLayer:[TestStarter node] atKey:varTestStarter_staticKey];
	[sharedScene_ addStaticLayerItemWithLayer:[TestLobby node] atKey:varTestLobby_staticKey];
	[sharedScene_ pushStaticLayerItemAtKey:varTestStarter_staticKey];
}

@end
