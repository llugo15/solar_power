import { Component, OnInit, OnDestroy } from '@angular/core';

import { Subscription } from 'rxjs';
import { AuthService } from '../auth/register/auth.service';
import { FirebaseService } from '../shared/firebase.service';
import { TestingService } from '../shared/testing.service';

@Component({
  selector: 'app-overview',
  templateUrl: './overview.page.html',
  styleUrls: ['./overview.page.scss'],
})
export class OverviewPage implements OnInit, OnDestroy {
  // subscriptions for the user and the appName
  private userSub: Subscription;
  private appNameSub: Subscription;

  // variable for authenticated user and systemName
  isAuth = false;
  systemName: string;

  constructor(private auth: AuthService,
              private readFirebase: FirebaseService,
              private writeFirebase: TestingService) { }

  ngOnInit() {
    // activating systemName, currentUser, and fetchSolar function before extracting data
    console.log("Loaded OverviewPage");
    this.readFirebase.fetchSystemName();
    this.writeFirebase.currentUser();
    this.readFirebase.fetchSolar();

    // getting the current user
    this.userSub = this.auth.user.subscribe(
      (user) => {
        this.isAuth = !!user; // this says true or false, true when we have a user and false otherwise
      }
    );

    // getting the appName
    this.appNameSub = this.readFirebase.appName.subscribe(
      (appName) => {
        this.systemName = appName;
      }
    );
    
  }

  // loging the user out after one hour
  logout() {
    this.auth.logout();
  }

  // writing the name the user names their system
  writeName() {
    this.writeFirebase.writeSystemName(this.systemName);
  }

  // destroying self made subscriptions
  ngOnDestroy() {
    this.userSub.unsubscribe();
    this.appNameSub.unsubscribe();
  }

}
