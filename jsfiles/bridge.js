function init_stockfish(skill, time) {
    stockfish.postMessage({eventType: "init_stockfish", eventData : [skill, time]});
}
function init_custom_stockfish(skill, time, customFen) {
    stockfish.postMessage({eventType: "init_custom_stockfish", eventData : [skill, time, customFen]});
}
function reinit_stockfish(skill, time) {
    stockfish.postMessage({eventType: "reinit_stockfish", eventData : [skill, time]});
}
function reinit_custom_stockfish(skill, time, customFen) {
    stockfish.postMessage({eventType: "reinit_custom_stockfish", eventData : [skill, time, customFen]});
}
function call_move(from, to) {
    stockfish.postMessage({eventType: "call_move", eventData : [from, to]});
}
function castle_move(castleSide) {
    stockfish.postMessage({eventType: "castle_move", eventData : [castleSide]});
}
function enpassant_move(from) {
    stockfish.postMessage({eventType: "enpassant_move", eventData : [from]});
}
function promotion_move(from, to, promoType) {
    stockfish.postMessage({eventType: "promotion_move", eventData : [from, to, promoType]});
}
function search_move() {
    stockfish.postMessage({eventType: "search_move", eventData : null});
}
function undo_move() {
    stockfish.postMessage({eventType: "undo_move", eventData : null});
}
function new_game() {
    stockfish.postMessage({eventType: "new_game", eventData : null});
}
function release_resource() {
    stockfish.postMessage({eventType: "release_resource", eventData : null});
}