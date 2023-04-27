import { Component, OnInit } from '@angular/core';
import { NgForm } from '@angular/forms';
import { AuthService } from './auth.service';

@Component({
  selector: 'app-register',
  templateUrl: './register.page.html',
  styleUrls: ['./register.page.scss'],
})
export class RegisterPage implements OnInit {
  isLoginMode = true;
  errorMessage = null;
  errorCode = null;

  constructor(private auth: AuthService
              ) { }

  ngOnInit() {
  }

  //  Used to switch from logging in to signing up
  onSwitchMode() {
    this.isLoginMode = !this.isLoginMode;
  }

  //  Used to submit the form
  onSubmit(form: NgForm) {
    if (!form.valid) {
      return;
    }
    // checking ifthe user is logged in or not and if there is an error
    if (this.isLoginMode) {
      this.auth.login(form.value.email, form.value.password)
      .catch((error) => {
        this.errorCode = error.code;
        this.errorMessage = error.message;
      });
    } else {
      this.auth.signup(form.value.email, form.value.password)
      .catch((error) => {
        this.errorCode = error.code;
        this.errorMessage = error.message;
      });
    }
    // resetting the form
    form.reset();
  }

  stopError() {
    this.errorMessage = false;
  }

}
