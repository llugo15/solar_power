import { Component, OnInit, ViewChild, ElementRef, OnDestroy } from '@angular/core';

import { Chart } from 'chart.js';
import { Subscription } from 'rxjs';
import { FirebaseService } from 'src/app/shared/firebase.service';
import { TestingService } from 'src/app/shared/testing.service';

@Component({
  selector: 'app-app-display',
  templateUrl: './app-display.component.html',
  styleUrls: ['./app-display.component.scss'],
})
export class AppDisplayComponent implements OnInit, OnDestroy {
  // variables used for storage and displaying to the user 
  powerData: number[] = [];
  timeData: string[] = [];
  currentData: number;
  voltageData: number;
  date: any;

  // variables needed for the battery display
  public buffer = 0.06;
  public progress = 0;
  progressDisplay = 0;
  charging: boolean = false;
  myNewChart: Chart<"line", any[], any>;

  // variables for the name of the device and keeping track of the data
  deviceName: string;
  powerProduced: number;
  batteryDischarge: number;
  batteryDischargeVol: number;
  batteryChargeVol: number;

  // subscriptions for listing to when the data changes
  userMacSub: Subscription;
  powerProducedSub: Subscription;
  activeVoltageSub: Subscription;
  activeCurrentSub: Subscription;
  batteryDischargeSub: Subscription;
  batteryDischargeSubVol: Subscription;
  batteryChargeSubVol: Subscription;
  chargingSub: Subscription;
  timeDataSub: Subscription;
  powerDataSub: Subscription;
  chartSub: Subscription;

  constructor(private readFirebase: FirebaseService,
              private writeFirebase: TestingService) { }
    
    ngOnInit() {
      // setting the current data and activating the batteryLvl 
      console.log("Loaded AppDisplayPage");
      this.date = new Date().toLocaleDateString();
      this.readFirebase.batteryLvl();

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

      // getting new imported  
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

      // getting charging value  
      this.chargingSub = this.readFirebase.isCharging.subscribe(
        (charge) => {
          this.charging = charge;
        }
      );

      // getting time array   
      this.timeDataSub = this.readFirebase.timeDataChange.subscribe(
        (time) => {
          this.timeData = time;
          this.displayDay();
        }
      );

      // getting power array 
      this.chartSub = this.readFirebase.solarChartData.subscribe(
        (power) => {
          this.powerData = power;
          this.displayDay();
        }
      );

      // getting the battery level from firebase
      this.readFirebase.batteryLevel.subscribe(
        (batteryLevel) => {
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
  
      // loading in the charts 
      this.loadChart()
    }

    
    // displaying the current day each time this website page is opened
    displayDay() {
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

    // updating the chart with week and month data
    updateChart(power: number[], time: string[]) {
      this.myNewChart.data.labels = this.timeData;
      this.myNewChart.data.datasets[0].data = this.powerData;
      this.myNewChart.update();
    }

  // used to control and give the graph its data
  ctx: CanvasRenderingContext2D;
  @ViewChild('powerGraph', {static: true}) powerGraph: ElementRef<HTMLCanvasElement>;
   loadChart() {
    this.ctx = this.powerGraph.nativeElement.getContext('2d');
    
    this.myNewChart = new Chart(this.ctx, {
      type: 'line',
      data: { 
        labels: [],
        datasets: [{
            label: "Power Output",
            data: [],
            borderWidth: 1,
        }]
      },
      options: {
        spanGaps: true,
        animation: false,
        plugins: {
          title: {
            display: true,
            text: 'Current System Power Output',
            color: '#000',
            font: {
              size: 12,
              family: 'Times New Roman',
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
                family: 'Times New Roman',
                size: 12,
                weight: 'normal',
                lineHeight: 1.2,
              },
          }
        },
        y: {
          title: {
            display: true,
            text: 'Output Power (v)',
            color: '#000',
            font: {
              family: 'Times New Roman',
              size: 12,
              weight: 'normal',
              lineHeight: 1.2,
            },
        },
        beginAtZero: true,
      }
        }
      }
    }); 
   }

   // have to destroy the subscriptions so there is no floating data that was allocated
   ngOnDestroy(): void {
     this.userMacSub.unsubscribe();
     this.powerProducedSub.unsubscribe();
     this.activeCurrentSub.unsubscribe();
     this.activeVoltageSub.unsubscribe();
     this.batteryDischargeSub.unsubscribe();
     this.batteryChargeSubVol.unsubscribe();
     this.batteryDischargeSubVol.unsubscribe();
     this.chargingSub.unsubscribe();
     this.timeDataSub.unsubscribe();
     this.powerDataSub.unsubscribe();
     this.chargingSub.unsubscribe();
   }

}
