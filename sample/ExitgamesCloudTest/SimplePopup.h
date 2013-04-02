//
//  SimplePopup.h
//  ExitgamesCloudTest
//
//  Created by Kim Jazz on 13. 2. 11..
//  Copyright (c) 2013년 Kim Jazz. All rights reserved.
//

#import "CMMHeader.h"

@interface SimplePopup : CMMPopupLayer{
	CCSprite *_backSprite;
}


@end

@interface SimplePopupAlert : SimplePopup{
	CCLabelTTF *labelContent;
	CMMMenuItemL *button1;
}

+(id)popupWithContent:(NSString *)content_;
-(id)initWithContent:(NSString *)content_;

@property (nonatomic, assign) NSString *content;
@property (nonatomic, readonly) CMMMenuItemL *button1;

@end

@interface SimplePopupConfirm : SimplePopupAlert{
	CMMMenuItemL *button2;
}

@property (nonatomic, readonly) CMMMenuItemL *button2;

@end

@interface SimpleLoadingLayer : CMMPopupLayer{
	CCLabelTTF *labelTitle;
}

@property (nonatomic, assign) NSString *title;

@end


#pragma mark - CMMScene+SimplePopupExtension

@interface CMMScene(SimplePopupExtension)

-(void)openPopupAlertWithContent:(NSString *)content_ closeBlock:(void(^)(SimplePopupAlert *alert_))closeBlock_;
-(void)openPopupAlertWithContent:(NSString *)content_;

-(void)openPopupConfirmWithContent:(NSString *)content_ closeBlock:(void(^)(SimplePopupConfirm *confirm_, BOOL result_))closeBlock_;

-(void)openLoadingLayerWithTitle:(NSString *)title_;
-(void)closeLoadingLayer;

@end