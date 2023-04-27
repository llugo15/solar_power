import { HttpClient, HttpHeaders, HttpParams} from '@angular/common/http';
import { Injectable } from '@angular/core';
import { environment } from 'src/environments/environment';
import { WeatherData } from './weather.model';
import { Observable } from 'rxjs';

@Injectable({
  providedIn: 'root'
})
export class WeatherService {

  constructor(private http: HttpClient) { }

  // getter for the current weather data for current city provided by the user
  getWeatherData(city: string): Observable<WeatherData> {
    return this.http.get<WeatherData>(environment.weatherApiBaseUrl + city,
                  {
                    headers: new HttpHeaders()
                    .set(
                      environment.XRapidAPIKeyHeaderName, environment.XRapidAPIKeyHeaderValue
                    )
                    .set(
                      environment.XRapidAPIHostHeaderName, environment.XRapidAPIHostHeaderValue
                    ),   
                  }
                );
  }
}