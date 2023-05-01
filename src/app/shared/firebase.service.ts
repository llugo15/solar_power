import { Injectable } from '@angular/core';

import { Database, ref, onValue, get, remove  } from '@angular/fire/database';
import { BehaviorSubject, Subject } from 'rxjs';
import { AuthService } from '../auth/register/auth.service';
import { User } from '../auth/register/user.model';
import { TestingService } from './testing.service';

@Injectable({
  providedIn: 'root'
})
export class FirebaseService {
  /*
  Difference between behavior subjects and subjects is that behavior remembers its last value. Cant be used with arrays. 
  */
  // used to emit data from the database to other components
  batteryLevel = new BehaviorSubject<number>(0);
  appName = new BehaviorSubject<string>(null);
  cityName = new BehaviorSubject<string>(null);
  producedPower = new BehaviorSubject<number>(0);
  userMAC = new BehaviorSubject<string>(null);
  activeVoltage = new BehaviorSubject<number>(0);
  activeCurrent = new BehaviorSubject<number>(0);
  totalBatteryDischarge = new BehaviorSubject<number>(0);
  totalBatteryDischargeVol = new BehaviorSubject<number>(0);
  totalBatteryChargeVol = new BehaviorSubject<number>(0);
  isCharging = new BehaviorSubject<boolean>(null);
  espConnected = new BehaviorSubject<boolean>(null);
  
  // used to emit data from database to other components
  timeDataChange = new Subject<string[]>();
  powerDatachanged = new Subject<number[]>();
  solarChartData = new Subject<number[]>();

  // variable arrays for data extract and sent out using subjects above
  timeData: string[] = [];
  solarPowerData: number[] = [];
  currentbatteryPower: number[] = [];
  currentbatteryVoltage: number[] = [];

  // keeping the count or setting names 
  voltage: number = 0;
  current: number = 0;
  totalDischarge: number = 0;
  totalChargeVol: number = 0;
  totalDisChargeVol: number = 0;
  powerProduced: number = 0;
  userMac: string;
  espMac: string;

  // used to store the current user
  user: User;


  constructor(private database: Database,
              private auth: AuthService) {}

  // fetches the current users system name from the database
  fetchSystemName() {
    this.auth.user.subscribe(
      (User) => {
        this.user = User;
        console.log('(TEST1) - User: ', this.user);
      }
    );
    let userEmail = this.user.email.split(".")[0];

    const databaseRef = ref(this.database, 'preferences/' + userEmail + '/systemName');
    onValue(databaseRef, (snapshot) => {
      console.log("(TEST2) - Current System: ", snapshot.val());
      this.appName.next(snapshot.val());
    });
  }

  // fetchs the current user's city name from the database
  fetchCityName() {
    this.auth.user.subscribe(
      (User) => {
        this.user = User;
      }
    );
    let userEmail = this.user.email.split(".")[0];

    const databaseRef = ref(this.database, 'preferences/' + userEmail + '/cityName');
    onValue(databaseRef, (snapshot) => {
      console.log("(TEST3) - City Name: ", snapshot.val());
      this.cityName.next(snapshot.val());
    });
  }

  // fetchs the current user's set mac address (defined by the user and not used currently)
  fetchUserMac() {
    this.auth.user.subscribe(
      (User) => {
        this.user = User;

      }
    );
    let userEmail = this.user.email.split(".")[0];

    const databaseRef = ref(this.database, 'preferences/' + userEmail + '/userMac');
    onValue(databaseRef, (snapshot) => {
      this.userMAC.next(snapshot.val());
      this.userMac = snapshot.val();
    });
  }

  // fetchs the mac address set by the esp32 to the backened (not used)
  fetchEspMac() {
    this.auth.user.subscribe(
      (User) => {
        this.user = User;
      }
    );
    let userEmail = this.user.email.split(".")[0];

    const databaseRef = ref(this.database, 'preferences/' + userEmail + '/espMac');
    onValue(databaseRef, (snapshot) => {
      this.espMac = snapshot.val();
    });
  }

  // fteches the current battery level from database
  batteryLvl() {
    this.auth.user.subscribe(
      (User) => {
        this.user = User;
      }
    );
    let userEmail = this.user.email.split(".")[0];
    const databaseRef = ref(this.database, userEmail + '/Battery');
    onValue(databaseRef,
      (batLvl) => {
        console.log("(TEST4) - Fetched BatteryLevel: ", batLvl.val().bat_vol);
        this.batteryLevel.next(batLvl.val());
      }
    );
  }

  // was going to be used to reset the data, this is how to track the amount of points during the day 
  resetData() {
    this.auth.user.subscribe(
      (User) => {
        this.user = User;
      }
    );
    let userEmail = this.user.email.split(".")[0];

    const databaseRef = ref(this.database, userEmail + '/solarpanel');
    // remove();
    console.log('In here');
  }

  // used for calaculating the power produced by the solar panel 
  solarPower(solarVol: number, solarCurr: number) {
    let solarPower: number;
  
    solarPower = (solarVol * solarCurr);
    this.powerProduced += solarPower;
    this.producedPower.next(+(this.powerProduced).toFixed(2));
    // console.log('SolarpowerData: ',this.solarPowerData);
    if (this.solarPowerData.length > 700) { this.resetData();  }
    this.solarPowerData.push(solarPower);

  }

  // main function that send most of the data
  fetchSolar() {  
    // gets the curren user
    this.auth.user.subscribe(
      (User) => {
        this.user = User;
      }
    );
    // set the current user's email without .com
    let userEmail = this.user.email.split(".")[0];

      // getting the solar panel and battery data from the database
      const databaseRef = ref(this.database, userEmail + '/solarpanel');
      // each time there is a change in this branch it re-evaluates
      onValue(databaseRef, (snapshot) => {
        // setting the current time each time new data comes in
        let currDate = new Date();
        let time = currDate.toLocaleTimeString('en-US', { hour: '2-digit', minute: '2-digit', });
        this.timeData.push(time);
        // console.log("Time data: ", this.timeData);
        console.log("(TEST6) - Solar Panel / Battery Data: ", snapshot.val());
  
        // re-setting these variables so that they dont double write
        this.powerProduced = 0;
        this.totalDischarge = 0;
        this.totalChargeVol = 0;
        this.totalDisChargeVol = 0;
        this.solarPowerData = [];
        this.currentbatteryPower = [];
        this.currentbatteryVoltage = [];
        
        // for loop iterating through the data to do calculations and emitting
        for (let data in snapshot.val()) {
          
          // console.log('Voltage: ', +(snapshot.val()[data].voltage), ' Current: ', +(snapshot.val()[data].current));
          // console.log('Voltage: ', +((+snapshot.val()[data].voltage).toFixed(2)), 
          // ' Current: ', +((+snapshot.val()[data].current).toFixed(2)));
          
          // emitting active votlage and current, feeding solar power function, feeding battery power and voltages arrays
          this.activeVoltage.next(+((+snapshot.val()[data].voltage).toFixed(2)));
          this.activeCurrent.next(+((+snapshot.val()[data].current).toFixed(2)));
          this.solarPower(+(snapshot.val()[data].voltage), +(snapshot.val()[data].current));
          this.currentbatteryPower.push(+((+snapshot.val()[data].batteryCurrent * +snapshot.val()[data].batteryVoltage).toFixed(2)));
          this.currentbatteryVoltage.push(+((+snapshot.val()[data].batteryVoltage).toFixed(2)));
          // this is going to be used to reset the data but has not been implemented
          if (this.currentbatteryVoltage.length > 1) { }
        
        // iterating through power data to set discharging or charging values / status 
      }
      for (let power = 1; power < this.currentbatteryPower.length; power++) {
        // console.log('Power: ', this.currentbatteryPower[power], ' Voltage: ', this.currentbatteryVoltage[power]);
        if (this.currentbatteryPower[power - 1] > this.currentbatteryPower[power]) { // if previous > current = discharging
          this.totalDischarge += this.currentbatteryPower[power];
          this.totalDisChargeVol += this.currentbatteryVoltage[power];
          this.isCharging.next(false);
        } else { // charging
          this.totalChargeVol += this.currentbatteryVoltage[power];
          this.isCharging.next(true);
        }
      }
      console.log("(TEST7) - Total Discharge Power: ", +(this.totalDischarge.toFixed(2)));
      console.log("(TEST8) - Total Discharge Voltage: ", +this.totalDisChargeVol.toFixed(2));
      console.log("(TEST9) - Total Charge Voltage: ", +this.totalChargeVol.toFixed(2));
      console.log("(TEST10) - Battery Power Array: ", this.currentbatteryPower.slice());
      console.log("(TEST11) - Time Data Array: ", this.timeData.slice());
      console.log("(TEST12) - Solar Power Data Array: ", this.solarPowerData.slice());

        // emitting all of the arrays that have been populated by the for loop above with two demical places
        this.totalBatteryDischarge.next(+(this.totalDischarge.toFixed(2)));
        this.totalBatteryDischargeVol.next(+this.totalDisChargeVol.toFixed(2));
        this.totalBatteryChargeVol.next(+this.totalChargeVol.toFixed(2));
        this.powerDatachanged.next(this.currentbatteryPower.slice());
        this.timeDataChange.next(this.timeData.slice());
        this.solarChartData.next(this.solarPowerData.slice());
      });

  }
 
}
