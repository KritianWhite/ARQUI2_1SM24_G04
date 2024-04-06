import { NgModule, CUSTOM_ELEMENTS_SCHEMA, NO_ERRORS_SCHEMA } from '@angular/core';
import { BrowserModule } from '@angular/platform-browser';

import { AppComponent } from './app.component';
import { BrowserAnimationsModule } from '@angular/platform-browser/animations';
import { MatCardModule } from '@angular/material/card';
import { MatInputModule } from '@angular/material/input';
import { MatFormFieldModule } from '@angular/material/form-field';
import { MatGridListModule } from '@angular/material/grid-list';
import { MatButtonModule } from '@angular/material/button';
import { MatSelectModule } from '@angular/material/select';
import { MatTableModule } from '@angular/material/table';
import {MatIconModule} from '@angular/material/icon';
import {
  MatDialog,
  MatDialogActions,
  MatDialogClose,
  MatDialogContent,
  MatDialogTitle,
} from '@angular/material/dialog';
import { IMqttServiceOptions, MqttModule } from 'ngx-mqtt';
import { NgCircleProgressModule } from 'ng-circle-progress';

export const connection: IMqttServiceOptions = {
  // Broker
  hostname: 'broker.emqx.io',
  port: 8083,
  path: '/mqtt',
  clean: true,
  connectTimeout: 4000,
  reconnectPeriod: 4000,
  // Credenciales de conexion
  clientId: 'mqttx_04be0b77_',
  username: '',
  password: '',
  protocol: 'ws',
  connectOnCreate: false,
}
@NgModule({
  declarations: [
    AppComponent
  ],
  imports: [
    BrowserModule,
    BrowserAnimationsModule,
    MatCardModule,
    MatFormFieldModule,
    MatInputModule,
    MatGridListModule,
    MatButtonModule,
    MatSelectModule,
    MatTableModule,
    MatIconModule,
    MqttModule.forRoot(connection),
    NgCircleProgressModule.forRoot({})
  ],
  schemas: [CUSTOM_ELEMENTS_SCHEMA, NO_ERRORS_SCHEMA],
  providers: [],
  bootstrap: [AppComponent]
})
export class AppModule { }
