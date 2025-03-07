import { Component, OnInit } from '@angular/core';
import { MovieService } from '../movie.service';
import { Movie, MovieResponse } from '../models/movie.model';

interface GenreWeights {
  [genreId: number]: number;
}

@Component({
  selector: 'app-recommend',
  templateUrl: './recommend.component.html',
  styleUrls: ['./recommend.component.css']
})
export class RecommendComponent implements OnInit {
  currentMovie: Movie | null = null;
  animationClass: string = ''; // for triggering CSS animation

  // Example genres; adjust these based on your app's genres
  availableGenres = [
    { id: 28, name: "Action" },
    { id: 12, name: "Adventure" },
    { id: 16, name: "Animation" },
    { id: 35, name: "Comedy" },
    { id: 18, name: "Drama" }
  ];

  // Initialize each genre with a default weight of 1
  genreWeights: GenreWeights = {};

  // Keep track of seen movies using a Set for fast lookup.
  seenMovieIds: Set<number> = new Set<number>();

  constructor(private movieService: MovieService) {}

  ngOnInit(): void {
    this.availableGenres.forEach(genre => {
      this.genreWeights[genre.id] = 1;
    });
    this.getNextRecommendation();
  }

  getNextRecommendation() {
    // Sort genres by weight (highest first)
    const sortedGenres = Object.entries(this.genreWeights).sort((a, b) => b[1] - a[1]);
    // Use the top 3 genre IDs for the recommendation
    const topGenreIds = sortedGenres.slice(0, 2).map(([id]) => id).join(',');
    
    this.movieService.getRecommendedMovies(topGenreIds).subscribe((response: MovieResponse) => {
      if (response.results && response.results.length > 0) {
        // Filter out movies already seen
        const unseenMovies = response.results.filter(movie => !this.seenMovieIds.has(movie.id));

        if (unseenMovies.length === 0) {
          // All movies in this batch have been seen.
          console.warn('All movies in the current batch have been seen. Resetting seen movies.');
          this.seenMovieIds.clear();
          // Use all movies if you want to allow repeats after reset
          this.selectRandomMovie(response.results);
        } else {
          this.selectRandomMovie(unseenMovies);
        }
      } else {
        this.currentMovie = null;
      }
    });
  }

  selectRandomMovie(movies: Movie[]) {
    const randomIndex = Math.floor(Math.random() * movies.length);
    let movie = movies[randomIndex];
    // Mark this movie as seen
    this.seenMovieIds.add(movie.id);
    // Build the full poster URL if needed.
    movie.poster_path = movie.poster_path 
      ? `https://image.tmdb.org/t/p/w500${movie.poster_path}` 
      : "/assets/img_gone.png";
    this.currentMovie = movie;
  }

  // Methods to simulate swipe with animation
  onClickLeft() {
    this.animationClass = 'slide-left';
    setTimeout(() => {
      this.onSwipeLeft();
      this.animationClass = '';
    }, 300); // duration matches CSS animation duration
  }

  onClickRight() {
    this.animationClass = 'slide-right';
    setTimeout(() => {
      this.onSwipeRight();
      this.animationClass = '';
    }, 300);
  }

  onSwipeLeft() {
    // Dislike: reduce the weight for each genre in current movie
    if (this.currentMovie && this.currentMovie.genre_ids) {
      this.currentMovie.genre_ids.forEach(genreId => {
        if (this.genreWeights[genreId] !== undefined) {
          this.genreWeights[genreId] = Math.max(this.genreWeights[genreId] - 0.1, 0);
        }
      });
    }
    this.getNextRecommendation();
  }

  onSwipeRight() {
    // Like: increase the weight for each genre in current movie
    if (this.currentMovie && this.currentMovie.genre_ids) {
      this.currentMovie.genre_ids.forEach(genreId => {
        if (this.genreWeights[genreId] !== undefined) {
          this.genreWeights[genreId] = Math.min(this.genreWeights[genreId] + 0.1, 2);
        }
      });
    }
    this.getNextRecommendation();
  }
}
