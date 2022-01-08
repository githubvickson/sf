mergeInto(LibraryManager.library, {
    cpp_init_stockfish : function(skill, time)
    {
        init_stockfish(skill, time);
    },
    cpp_init_custom_stockfish : function(skill, time, customFEN)
    {
        init_custom_stockfish(skill, time, Pointer_stringify(customFEN));
    },
    cpp_re_init_stockfish : function(skill, time)
    {
        reinit_stockfish(skill, time);
    },
    cpp_re_init_custom_stockfish : function(skill, time, customFEN)
    {
        reinit_custom_stockfish(skill, time, Pointer_stringify(customFEN));
    },
    cpp_call_move : function(from, to)
    {
        call_move(from, to);
    },
    cpp_castle_move : function(castleSide)
    {
        castle_move(castleSide);
    },
    cpp_enpassant_move : function(from)
    {
        enpassant_move(from);
    },
    cpp_promotion_move : function(from, to, promoType)
    {
        promotion_move(from, to, promoType);
    },
    cpp_search_move : function()
    {
        return search_move();
    },
    cpp_undo_move : function()
    {
        undo_move();
    },
    cpp_new_game : function()
    {
        new_game();
    },
    cpp_release_resource : function()
    {
        release_resource();
    },
});