// This file can be replaced during build by using the `fileReplacements` array.
// `ng build --prod` replaces `environment.ts` with `environment.prod.ts`.
// The list of file replacements can be found in `angular.json`.
/* 
  Environment is used to set the projects details for angular.
  Under the environemnt block are the keys that are needed to get the RapidAPI Weather API to work,
  makes it easier to establish variables and make calls in weather service
*/
export const environment = {
  production: false,
  firebase: {
    projectId: 'test-1ecd1',
    appId: '1:828440307220:web:d5dbc6df677f688b73a901',
    databaseURL: 'https://test-1ecd1-default-rtdb.firebaseio.com',
    storageBucket: 'test-1ecd1.appspot.com',
    apiKey: 'AIzaSyAgzEHqO_vdPP_Zh_rr6VQn-KRbJ-wpHAA',
    authDomain: 'test-1ecd1.firebaseapp.com',
    messagingSenderId: '828440307220',
    measurementId: 'G-VD80QSN9ZC',
  },
  weatherApiBaseUrl: 'https://open-weather13.p.rapidapi.com/city/',
  XRapidAPIHostHeaderName: 'X-RapidAPI-Host',
  XRapidAPIHostHeaderValue: 'open-weather13.p.rapidapi.com',
  XRapidAPIKeyHeaderName: 'X-RapidAPI-Key',
  XRapidAPIKeyHeaderValue: '8383b13f9amsh819f5ce0e332ba7p1e9743jsn109cbdc32d94'
};

