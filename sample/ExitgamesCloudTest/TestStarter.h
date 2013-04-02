//  Created by JGroup(kimbobv22@gmail.com)

#import "CMMHeader.h"
#import "SimplePopup.h"
#import "JGPhotonCloudClient.h"

#define varTestStarter_staticKey @"_TestStarter_"

@interface TestStarter : CMMLayer<JGPhotonCloudCommonDelegate>{
	CMMControlItemText *textFieldUserName;
	CMMMenuItemL *btnConnect;
}

-(void)_connectServer;

@end
