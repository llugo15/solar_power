import { Component, OnInit } from '@angular/core';
import { AuthService } from './auth/register/auth.service';
import { FirebaseService } from './shared/firebase.service';
import { TestingService } from './shared/testing.service';

@Component({
  selector: 'app-root',
  templateUrl: 'app.component.html',
  styleUrls: ['app.component.scss'],
})
export class AppComponent implements OnInit {
  // variables for testing the backend (mocking the ESP32)
  dummyVoltage: number;
  dummyCurrent: number;
  dummyBatCurrent: number;
  dummyBatVoltage: number;

  constructor(private auth: AuthService,
              private testbase: TestingService) {}

    ngOnInit() {
      // auto logins the user on refresh 
      this.auth.autoLogin();

      // setInterval(() => {
      //   this.dummyCurrent = +(Math.random() * 3).toFixed(2);
      //   this.dummyVoltage = +(Math.random() * 60).toFixed(2);
      //   this.dummyBatVoltage = +(Math.random() * 3).toFixed(2);
      //   this.dummyBatCurrent = +(Math.random() * 60).toFixed(2);
      //   this.testbase.testBackend(this.dummyBatVoltage, this.dummyBatCurrent, this.dummyVoltage, this.dummyCurrent);
      // }, 2000);


    }
    
}
