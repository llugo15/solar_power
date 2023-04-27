import { NgModule } from '@angular/core';
import { Routes, RouterModule } from '@angular/router';

import { OverviewPage } from './overview.page';
import { SystemPage } from './system/system.page';
import { WeatherDisplayComponent } from './weather-display/weather-display.component';

const routes: Routes = [
  {
    path: '',
    component: OverviewPage
  },
  {
    path: 'system',
    component: SystemPage
  },
  {
    path: 'weather',
    component: WeatherDisplayComponent
  }
];

@NgModule({
  imports: [RouterModule.forChild(routes)],
  exports: [RouterModule],
})
export class OverviewPageRoutingModule {}
