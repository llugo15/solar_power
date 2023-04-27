import { Component, OnInit, ViewChild, ElementRef } from '@angular/core';

import { Chart } from 'chart.js';
import { f } from 'chart.js/dist/chunks/helpers.core';
import { Subscription } from 'rxjs';
import { FirebaseService } from 'src/app/shared/firebase.service';
import { TestingService } from 'src/app/shared/testing.service';

@Component({
  selector: 'app-web-display',
  templateUrl: './web-display.component.html',
  styleUrls: ['./web-display.component.scss'],
})
export class WebDisplayComponent implements OnInit {
  // used to store the power and time data from database 
  powerData: number[] = [];
  timeData: string[] = [];

  // name for system
  systemName: string; 

  // name for device
  deviceName: string; 

  // used to display data to the user 
  currentData: number;
  voltageData: number;
  powerProduced: number;
  batteryChargeVol: number;
  batteryDischargeVol: number;
  batteryDischarge: number;

  // variables needed for date and dynamic battery loading 
  date: any;
  public buffer = 0.06;
  public progress = 0;
  progressDisplay = 0;
  charging: boolean = true;
  espConnected: boolean = true;
  myNewChart: Chart<"line", any[], any>;

  // subscribing to all the data from the backend
  appNameSub: Subscription;
  powerDataSub: Subscription;
  timeDataSub: Subscription;
  currentDataSub: Subscription;
  voltageDataSub: Subscription;
  powerDataSub2: Subscription;
  batterylvlSub: Subscription;
  userMacSub: Subscription;
  powerProducedSub: Subscription;
  activeVoltageSub: Subscription;
  activeCurrentSub: Subscription;
  batteryDischargeSub: Subscription;
  batteryDischargeSubVol: Subscription;
  batteryChargeSubVol: Subscription;
  chargingSub: Subscription;
  espConnectSub: Subscription;
  chartData: Subscription;
  averagePower: number = 0;

  constructor(private readFirebase: FirebaseService,
              private writeFirebase: TestingService) { }         
    
    ngOnInit() {
      // setting the local time to be displayed and activating the batteryLvl function
      console.log("Loaded WebsiteDisplayPage");
      this.date = new Date().toLocaleDateString();
      this.readFirebase.batteryLvl();
    
      // getting system name 
      this.appNameSub = this.readFirebase.appName.subscribe(
        (appName) => {
          this.systemName = appName;
        }
      );

      // getting mac name 
      this.userMacSub = this.readFirebase.userMAC.subscribe(
        (mac) => {
          this.deviceName = mac;
        }
      );

      // running total of power produced
      this.powerProducedSub = this.readFirebase.producedPower.subscribe(
        (power) => {
          this.powerProduced = power;
        }
      );

      // getting active voltage 
      this.activeVoltageSub = this.readFirebase.activeVoltage.subscribe(
        (voltage) => {
          this.voltageData = voltage;
        }
      );

      // getting active current 
      this.activeCurrentSub = this.readFirebase.activeCurrent.subscribe(
        (current) => {
          this.currentData = current;
        }
      );

      // getting net imported  
      this.batteryDischargeSub = this.readFirebase.totalBatteryDischarge.subscribe(
        (discharge) => {
          this.batteryDischarge = discharge;
        }
      );

      // getting discharge voltage  
      this.batteryDischargeSubVol = this.readFirebase.totalBatteryDischargeVol.subscribe(
        (discharge) => {
          this.batteryDischargeVol = discharge;
        }
      );

      // getting charge voltage  
      this.batteryChargeSubVol = this.readFirebase.totalBatteryChargeVol.subscribe(
        (charge) => {
          this.batteryChargeVol = charge;
        }
      );

      // getting isCharging value   
      this.chargingSub = this.readFirebase.isCharging.subscribe(
        (charge) => {
          this.charging = charge;
        }
      );

      // getting isCharging value   
      this.espConnectSub = this.readFirebase.espConnected.subscribe(
        (esp) => {
          this.espConnected = esp;
        }
      );

      // getting time array   
      this.timeDataSub = this.readFirebase.timeDataChange.subscribe(
        (time) => {
          this.timeData = time;
          this.displayDay(); // actives data on the chart
        }
      );

      // getting power array 
      this.chartData = this.readFirebase.solarChartData.subscribe(
        (power) => {
          this.powerData = power;
          this.displayDay(); // activates data on the chart 
        }
      );

      // getting the batterylevel and changing battery display dynamically 
      this.batterylvlSub = this.readFirebase.batteryLevel.subscribe(
        (batteryLevel) => {
          this.progress <= batteryLevel ? this.charging : !this.charging;
          this.progress = Math.floor(batteryLevel);
          switch (this.progress) {
            case 0:
              this.progressDisplay = 0;
              break;
            case 1:
              this.progressDisplay = 0.08;
              break;
            case 2:
              this.progressDisplay = 0.16;
              break;
            case 3:
              this.progressDisplay = 0.25;
              break;
            case 4:
              this.progressDisplay = 0.33;
              break;
            case 5:
              this.progressDisplay = 0.42;
              break;
            case 6:
              this.progressDisplay = 0.50;
              break;
            case 7:
              this.progressDisplay = 0.58;
              break;
            case 8:
              this.progressDisplay = 0.66;
              break;
            case 9:
              this.progressDisplay = 0.75;
              break;
            case 10:
              this.progressDisplay = 0.83;
              break;
            case 11:
              this.progressDisplay = 0.91;
              break;
            case 12:
              this.progressDisplay = 1;
              break;
          }
          this.buffer = this.progressDisplay + 0.02;
        }
        );
      // loading the chart once all the data has been collected 
      this.loadChart();
    }

  // writing the system name for website
  writeName() {
    this.writeFirebase.writeSystemName(this.systemName);
  }

  // displaying the current day each time this website page is opened
  displayDay() {
    // console.log('!!!TimeData: ', this.timeData, ' PowerData: ', this.powerData);
    this.date = new Date().toLocaleString( 'default', {weekday: 'long'});
    this.updateChart(this.powerData, this.timeData);
  }

  // this is for the week graph 
  displayWeek() {
    let fakeWeekTime: string[] = ['M', 'T', 'W', 'Th', 'F', 'Sa', 'Su'];
    let fakeWeekPower: number[] = [0, 0, 0, 0, 0, 0, 0];
    this.date = new Date().toLocaleDateString();
    this.updateChart(fakeWeekPower, fakeWeekTime);
  }

  // this updates the chart so that multiple data can be displayed
  updateChart(power: number[], time: string[]) {
    this.myNewChart.data.labels = time;
    this.myNewChart.data.datasets[0].data = power;
    this.myNewChart.update();
  }
  
  // the graph that is being made and used to display data for day, week, and month
  ctx: CanvasRenderingContext2D;
  @ViewChild('powerGraph', {static: true}) powerGraph: ElementRef<HTMLCanvasElement>;
   loadChart() {
    this.ctx = this.powerGraph.nativeElement.getContext('2d');
    
    this.myNewChart = new Chart(this.ctx, {
      type: 'line',
      data: { 
        labels: this.timeData,
        datasets: [{
            label: "Output Power",
            borderWidth: 1,
            data: this.powerData
        }]
      },
      options: {
        spanGaps: true,
        animation: false,
        responsive: true,
        plugins: {
          title: {
            display: true,
            text: 'Current System Power Output',
            color: '#000',
            font: {
              size: 12,
              family: 'Segoe UI',
              weight: 'normal'
            },
            padding: {
              top: 0
            }
          },
        },
        scales: {
          x: {
              title: {
                display: true,
                text: 'Time Frame',
                color: '#000',
                font: {
                  family: 'Segoe UI',
                  size: 12,
                  weight: 'normal',
                  lineHeight: 1.2,
                },
            }
          },
          y: {
            title: {
              display: true,
              text: 'Output Power (W)',
              color: '#000',
              font: {
                family: 'Segoe UI',
                size: 12,
                weight: 'normal',
                lineHeight: 1.2,
              },
          },
          beginAtZero: true,
        }
      }
    },
  }

    ); 
   }


  // always destroy user made subscriptions 
  ngOnDestroy() {
    this.appNameSub.unsubscribe();
    this.powerDataSub.unsubscribe();
    this.timeDataSub.unsubscribe();
    this.currentDataSub.unsubscribe();
    this.voltageDataSub.unsubscribe();
    this.powerDataSub2.unsubscribe();
    this.batterylvlSub.unsubscribe();
    this.userMacSub.unsubscribe();
    this.powerProducedSub.unsubscribe();
    this.activeCurrentSub.unsubscribe();
    this.batteryDischargeSub.unsubscribe();
    this.chargingSub.unsubscribe();
    this.espConnectSub.unsubscribe();
    this.powerDataSub.unsubscribe();
    this.timeDataSub.unsubscribe();
    this.chartData.unsubscribe();
  }
}


function getGradient(ctx: CanvasRenderingContext2D, chartArea: f): import("chart.js/dist/chunks/helpers.core").ck {
  throw new Error('Function not implemented.');
}

