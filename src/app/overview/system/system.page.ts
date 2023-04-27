import { Component, OnInit, ViewChild, ElementRef } from '@angular/core';

import { Chart } from 'chart.js';
import { FirebaseService } from 'src/app/shared/firebase.service';

@Component({
  selector: 'app-system',
  templateUrl: './system.page.html',
  styleUrls: ['./system.page.scss'],
})
export class SystemPage implements OnInit {
  monthDate: string;
  weekDate: string;

  constructor() { }

  
  // getting the data from the database and the current month, and week day
  ngOnInit() {
    console.log("Loaded SystemPage");
    this.weekDate = new Date().toLocaleString( 'default', {weekday: 'long'});
    this.monthDate = new Date().toLocaleString( 'default', {month: 'long'});
  }

}
  