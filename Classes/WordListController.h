//
//  WordListController.h
//  EnBasic
//
//  Created by Oleksandr Iakovliev on 7/17/10.
//  Copyright 2010 LynxLine. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface WordListController : UITableViewController {
    UINavigationController *navigationController;
    NSMutableArray * words;
}

@property (nonatomic, retain) IBOutlet UINavigationController *navigationController;

@end
