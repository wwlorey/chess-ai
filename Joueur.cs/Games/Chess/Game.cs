// The traditional 8x8 chess board with pieces.

// DO NOT MODIFY THIS FILE
// Never try to directly create an instance of this class, or modify its member variables.
// Instead, you should only be reading its variables and calling its functions.

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;


/// <summary>
/// The traditional 8x8 chess board with pieces.
/// </summary>
namespace Joueur.cs.Games.Chess
{
    /// <summary>
    /// The traditional 8x8 chess board with pieces.
    /// </summary>
    public class Game : BaseGame
    {
        #region Properties
        /// <summary>
        /// Forsyth-Edwards Notation (fen), a notation that describes the game board state.
        /// </summary>
        public string Fen { get; protected set; }

        /// <summary>
        /// The list of [known] moves that have occured in the game, in Standard Algebriac Notation (SAN) format. The first element is the first move, with the last being the most recent.
        /// </summary>
        public IList<string> History { get; protected set; }

        /// <summary>
        /// List of all the players in the game.
        /// </summary>
        public IList<Chess.Player> Players { get; protected set; }

        /// <summary>
        /// A unique identifier for the game instance that is being played.
        /// </summary>
        public string Session { get; protected set; }



        #endregion


        #region Methods
        /// <summary>
        /// Creates a new instance of Game. Used during game initialization, do not call directly.
        /// </summary>
        protected Game() : base()
        {
            this.Name = "Chess";

            this.History = new List<string>();
            this.Players = new List<Chess.Player>();
        }



        #endregion
    }
}
