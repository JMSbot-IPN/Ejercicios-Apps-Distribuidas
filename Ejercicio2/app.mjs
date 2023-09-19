import express from 'express';
import passport from 'passport';
import { Strategy as SpotifyStrategy } from 'passport-spotify';
import session from 'express-session';
import crypto from 'crypto';
import fetch from 'node-fetch';

const app = express();

// Generate a random secret key
const secretKey = crypto.randomBytes(32).toString('hex');
console.log(`Your secret key: ${secretKey}`);

// Configure session middleware
app.use(session({ secret: secretKey, resave: true, saveUninitialized: true }));

// Initialize Passport
app.use(passport.initialize());
app.use(passport.session());

// Configure the Spotify Strategy
passport.use(
  new SpotifyStrategy(
    {
      clientID: '4ccd9e31e20947d38e023627a4432200',
      clientSecret: '59321065746f4418b51154fae4dd79d4',
      callbackURL: 'http://localhost:8888/callback',
    },
    (accessToken, refreshToken, expires_in, profile, done) => {
      // Store the access token and user profile
      // You can save them in a database or session
      return done(null, { profile, accessToken });
    }
  )
);

// Serialize user to store in session
passport.serializeUser((user, done) => {
  done(null, user);
});

// Deserialize user from session
passport.deserializeUser((user, done) => {
  done(null, user);
});

// Redirect the user to Spotify for authentication
app.get('/auth/spotify', passport.authenticate('spotify', { scope: ['user-read-private', 'playlist-read-private'] }));

// Callback route after successful authentication
app.get(
  '/callback',
  passport.authenticate('spotify', { failureRedirect: '/' }),
  (req, res) => {
    res.redirect('/getPlaylists'); // Redirect to a route that fetches user playlists
  }
);

// Add a route to fetch user playlists
app.get('/getPlaylists', (req, res) => {
  const { accessToken } = req.user; // Get the access token from the user's session

  // Use the access token to make an API request to fetch user playlists
  fetch('https://api.spotify.com/v1/me/playlists', {
    method: 'GET',
    headers: {
      Authorization: `Bearer ${accessToken}`,
    },
  })
    .then((response) => response.json())
    .then((data) => {
      res.json(data);
    })
    .catch((error) => {
      console.error(error);
      res.status(500).send('Error fetching playlists');
    });
});

// Define a route handler for the root path
app.get('/', (req, res) => {
  res.send('Raiz de la aplicacion de Spotify');
});

// Start the server
app.listen(8888, () => {
  console.log('Server is running on port 8888');
});