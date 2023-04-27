import { Component, OnInit, OnDestroy } from '@angular/core';
import { Subscription } from 'rxjs';
import { FirebaseService } from 'src/app/shared/firebase.service';
import { TestingService } from 'src/app/shared/testing.service';
import { WeatherService } from 'src/app/shared/weather.service';

@Component({
  selector: 'app-weather-display',
  templateUrl: './weather-display.component.html',
  styleUrls: ['./weather-display.component.scss'],
})
export class WeatherDisplayComponent implements OnInit, OnDestroy {

  constructor(private weather: WeatherService,
              private readFirebase: FirebaseService,
              private writeFirebase: TestingService) { }

  // variables to hold the data gotten from the RapidAPI weather service
  currentTemp: number = 0;
  maxTemp: number = 0;
  minTemp: number = 0;
  wind: number = 0;
  clouds: number = 0;
  description: string= '';
  cityName: string;
  isHot;

  // subscribing to the city subject
  cityNameSub: Subscription;

  ngOnInit() {
    // activiating the city name function
    this.readFirebase.fetchCityName();
    // getting the current city name 
    this.cityNameSub = this.readFirebase.cityName.subscribe(
      (cityName) => {
        this.cityName = cityName;

        // setting the current weather from the weather service and getting back the data from that city
        this.weather.getWeatherData(this.cityName)
        .subscribe({ next: (response) => {
          this.currentTemp = response.main.temp;
          this.maxTemp = response.main.temp_max;
          this.minTemp = response.main.temp_min;
          this.wind = response.wind.speed;
          this.description = response.weather[0].description;
          this.clouds = response.clouds.all;

          if (this.currentTemp > 70) { this.isHot = true; }
          else { this.isHot = false; }
          }
        });
      }

    );

    // console.log("Inside Weather: ", this.cityName);

  }

  // writing the name the user names their system
  writeName() {
    this.writeFirebase.writeCityName(this.cityName);
  }

  // destroying the subscription made to the city name
  ngOnDestroy(): void {
    this.cityNameSub.unsubscribe();
  }

}
