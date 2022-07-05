//
//  EnBasicAppDelegate.h
//  EnBasic
//
//  Created by Oleksandr Iakovliev on 7/17/10.
//  Copyright LynxLine 2010. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface EnBasicAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    UINavigationController *navigationController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet UINavigationController *navigationController;

@end
