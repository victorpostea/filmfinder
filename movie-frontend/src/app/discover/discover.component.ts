import { Component } from '@angular/core';
import { MovieService } from '../movie.service';
import { Observable } from 'rxjs';
import { Movie, MovieResponse } from '../models/movie.model';
import { map } from 'rxjs/operators';

@Component({
  selector: 'app-discover',
  templateUrl: './discover.component.html',
  styleUrls: ['./discover.component.css']
})
export class DiscoverComponent {
  selectedGenre: string = '';
  movie_list: Observable<MovieResponse> | null = null;
  
  constructor(private movieservice: MovieService) {}
  
  discoverFilms() {
    if (this.selectedGenre) {
      console.log("Discovering movies for genre:", this.selectedGenre);
      this.movie_list = this.movieservice.discoverMovies(this.selectedGenre).pipe(
        map(response => this.parseData(response))
      );
    }
  }
  
  parseData(response: MovieResponse): MovieResponse {
    if (!response.results) {
      response.results = [];
      return response;
    }
    
    response.results = response.results.map(movie => ({
      ...movie,
      title: movie.title || "Unknown Title",
      release_date: movie.release_date || "N/A",
      vote_average: typeof movie.vote_average === "number" ? movie.vote_average : 0,
      overview: movie.overview || "No overview available.",
      // Build full poster URL or use fallback if not available
      poster_path: movie.poster_path 
        ? `https://image.tmdb.org/t/p/w500${movie.poster_path}` 
        : "/assets/img_gone.png"
    }));
  
    return response;
  }
}
