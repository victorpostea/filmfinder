import { Component } from '@angular/core';
import { MovieService } from '../movie.service';
import { Observable } from 'rxjs';
import { Movie, MovieResponse } from '../models/movie.model';

@Component({
  selector: 'app-home',
  templateUrl: './home.component.html',
  styleUrls: ['./home.component.css']
})

export class HomeComponent {
  userInput: string = '';
  movie_list: Observable<MovieResponse> | null = null;

  constructor(private movieservice: MovieService){}

  submitInput() {
    console.log("user input submitted: ", this.userInput);
    this.findFilms();
  }

  findFilms() {
    if (this.userInput) {
      this.movie_list = this.movieservice.searchMovie(this.userInput);
    }
  }

}
