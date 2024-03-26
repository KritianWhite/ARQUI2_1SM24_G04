import { NgModule } from '@angular/core';
import { BrowserModule } from '@angular/platform-browser';

import { AppComponent } from './app.component';
import { BrowserAnimationsModule } from '@angular/platform-browser/animations';
import { MatCardModule } from '@angular/material/card';
import { MatInputModule } from '@angular/material/input';
import { MatFormFieldModule } from '@angular/material/form-field';
import { MatGridListModule } from '@angular/material/grid-list';
import { MatButtonModule } from '@angular/material/button';
import { MatSelectModule } from '@angular/material/select';
import { IMqttServiceOptions, MqttModule } from 'ngx-mqtt';

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
    MqttModule.forRoot(connection)
  ],
  providers: [],
  bootstrap: [AppComponent]
})
export class AppModule { }
