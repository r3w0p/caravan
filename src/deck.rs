use rand::thread_rng;
use rand::seq::SliceRandom;

const DECK_STANDARD_MAX: u8 = 52;
const DECK_CARAVAN_MIN: u8 = 30;
const DECK_CARAVAN_MAX: u8 = 156;
const SAMPLE_DECKS_MIN: u8 = 1;
const SAMPLE_DECKS_MAX: u8 = 3;

#[derive(Debug)]
pub enum DeckError {
    NumCards, NumSamples
}

#[derive(Clone, Copy)]
pub enum Suit {
    NoSuit, Clubs, Diamonds, Hearts, Spades
}

impl Suit {
    pub fn to_str(&self) -> &str {
        match &self {
            Suit::NoSuit => "",
            Suit::Clubs => "CLUBS",
            Suit::Diamonds => "DIAMONDS",
            Suit::Hearts => "HEARTS",
            Suit::Spades => "SPADES"
        }
    }
}

pub enum Rank {
    Ace, Two, Three, Four, Five, Six, Seven, Eight, Nine, Ten,
    Jack, Queen, King, Joker
}

impl Rank {
    pub fn to_str(&self) -> &str {
        match &self {
            Rank::Ace => "ACE",
            Rank::Two => "TWO",
            Rank::Three => "THREE",
            Rank::Four => "FOUR",
            Rank::Five => "FIVE",
            Rank::Six => "SIX",
            Rank::Seven => "SEVEN",
            Rank::Eight => "EIGHT",
            Rank::Nine => "NINE",
            Rank::Ten => "TEN",
            Rank::Jack => "JACK",
            Rank::Queen => "QUEEN",
            Rank::King => "KING",
            Rank::Joker => "JOKER",
        }
    }
}

pub struct Card {
    pub suit: Suit,
    pub rank: Rank
}

pub type Deck = Vec<Card>;

pub fn build_caravan_deck(
    num_cards: u8,
    num_sample_decks: u8,
    balanced: bool) -> Result<Deck, DeckError> {

    let total_sample_cards: u8;
    let sample_decks: [Deck; SAMPLE_DECKS_MAX as usize] = [];
    
    if num_cards < DECK_CARAVAN_MIN { Err(DeckError::NumCards) }
    if num_cards > DECK_CARAVAN_MAX { Err(DeckError::NumCards) }

    if num_sample_decks < SAMPLE_DECKS_MIN { Err(DeckError::NumSamples) }
    if num_sample_decks > SAMPLE_DECKS_MAX { Err(DeckError::NumSamples) }

    total_sample_cards = num_sample_decks * DECK_STANDARD_MAX;

    if total_sample_cards < num_cards { Err(DeckError::NumCards) }

    for i in 0..num_sample_decks {
        sample_decks[i] = build_standard_deck(true);
    }

    if balanced {

    } else {

    }

    Ok(build_standard_deck(false))  // TODO
}

fn build_standard_deck(shuffle: bool) -> Deck {
    let mut d: Deck = Deck::new();

    for s in [
        Suit::Clubs,
        Suit::Diamonds,
        Suit::Hearts,
        Suit::Spades
    ] {
        for r in [
            Rank::Ace,
            Rank::Two,
            Rank::Three,
            Rank::Four,
            Rank::Five,
            Rank::Six,
            Rank::Seven,
            Rank::Eight,
            Rank::Nine,
            Rank::Ten,
            Rank::Jack,
            Rank::Queen,
            Rank::King,
        ] {
            d.push(Card {
                suit: s,
                rank: r
            });
        }
    }

    for _ in 0..2 {
        d.push(Card {
            suit: Suit::NoSuit,
            rank: Rank::Joker
        });
    }

    if shuffle {
        d.shuffle(&mut thread_rng());
    }

    d
}
