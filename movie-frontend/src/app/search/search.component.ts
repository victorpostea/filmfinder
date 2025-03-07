import { Component } from '@angular/core';
import { MovieService } from '../movie.service';
import { Observable } from 'rxjs';
import { Movie, MovieResponse } from '../models/movie.model';

@Component({
  selector: 'app-search',
  templateUrl: './search.component.html',
  styleUrls: ['./search.component.css']
})
export class SearchComponent {
  userInput: string = '';
  queryParam: string = '';
  movie_list: Observable<MovieResponse> | null = null;

  constructor(private movieservice: MovieService){}

  submitInput() {
    console.log("user input submitted: ", this.userInput);
    this.queryParam = this.userInput.replace(/ /g, "&20")
    this.findFilms();
  }

  findFilms() {
    if (this.userInput) {
      this.movie_list = this.movieservice.searchMovie(this.queryParam);
    }
  }
}
