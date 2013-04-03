#Version 0.3.0 - updated (2013.04.03)
* [FIX] **JGPhotonCloudClient** - change process

		@protocol JGPhotonCloudCommonDelegate <NSObject>
		…
		//removed
		-(void)jgPhotonCloudJoinedLobby:(JGPhotonCloudClient *)client_;
		…
		@end
		
		@protocol JGPhotonCloudLobbyDelegate <NSObject>
		…
		//added
		-(void)jgPhotonCloudJoinedLobby:(JGPhotonCloudClient *)client_;
		…
		@end

#Version 0.2.0 - updated (2013.04.02)
* [NEW] **JGPhotonCloudClient** - add method can send event to peer individually
* [FIX] **JGPhotonCloudClient** - fix method name `sendEventWithEventCode~` to `sendEventToAllWithEventCode~`
* [NEW] update phton cloud library version (`v3.0.4.4` -> `v3.2.1.1`)

#Version 0.1.0 - updated (2013.02.14)
* [NEW] first push
