import { Injectable } from '@angular/core';

import { Database, push, ref, update} from '@angular/fire/database';
import { AuthService } from '../auth/register/auth.service';
import { User } from '../auth/register/user.model';

@Injectable({
  providedIn: 'root'
})
export class TestingService {
  // variable for the current user and their email
  User: User;
  userEmail: string;
  
  constructor(private database: Database,
              private auth: AuthService) {}

  // used to test backend database (use push for arrays and use update for replacing)
  testBackend(bat_vol: number, bat_curr: number, sp_vol: number, sp_curr: number) {
    // getting current user
    this.auth.user.subscribe(
      (user) => {
        this.User = user;
      }
    );
    // extracting their email without .com
    this.userEmail = this.User.email.split(".")[0];

    // setting the URL to send to
    const userData = ref(this.database, this.userEmail);
    const testsolarData = ref(this.database, this.userEmail + "/solarpanel");

    // pushing the data that we are wanting to test (solar / battery data)
    push(testsolarData, 
      {
        voltage: sp_vol,
        current: sp_curr,
        batteryVoltage: bat_vol,
        batteryCurrent: bat_curr,
      }
    );
    
    // updating the battery voltage 
    update(userData, 
      {
        Battery: bat_vol
      }
    )
  }

  // used to write the system name that is set by the current user
  writeSystemName(name: string) {
    this.auth.user.subscribe(
      (user) => {
        this.User = user;
        console.log('(9)User: ', this.User);

      }
    );

    this.userEmail = this.User.email.split(".")[0];
    const userSystemName = ref(this.database, "preferences/" + this.userEmail);

    if (name !== "") {
      update(userSystemName,
        {
          systemName: name
        }
      );
      }
  }

  // writes the city name for the weather for the current user set by the user
  writeCityName(name: string) {
    this.auth.user.subscribe(
      (user) => {
        this.User = user;
        console.log('(10)User: ', this.User);

      }
    );

    this.userEmail = this.User.email.split(".")[0];
    const userSystemName = ref(this.database, "preferences/" + this.userEmail);

    if (name !== "") {
      update(userSystemName,
        {
          cityName: name
        }
      );
      }
  }

  // setting the current user that is loged-in 
  currentUser() {
    this.auth.user.subscribe(
      (user) => {
        this.User = user;
        console.log('(11)User: ', this.User);
      }
    );

    this.userEmail = this.User.email.split(".")[0];
    const userSystemName = ref(this.database);

    if (this.userEmail !== "") {
      update(userSystemName,
        {
          currentUser: this.userEmail
        }
      );
      }
  }

}
