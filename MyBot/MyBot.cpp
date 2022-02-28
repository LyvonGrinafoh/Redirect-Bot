#include <dpp/dpp.h>
#include <stdlib.h>
#include <stdio.h>
#include <dpp/message.h>

const std::string BOT_TOKEN = dpp::utility::read_file("Token.txt");
const dpp::snowflake MY_GUILD_ID = 907527594096853002;

int main()
{

    dpp::cluster bot(BOT_TOKEN, dpp::intents::i_default_intents | dpp::intents::i_guild_messages | dpp::intents::i_all_intents);

    bot.on_log(dpp::utility::cout_logger());

    bot.on_message_create([&](const dpp::message_create_t& ev) {

        if (ev.msg.author.is_bot() == false && ev.msg.attachments.size() > 0) {

            for (size_t x = 0; x < ev.msg.attachments.size(); x++) {

                ev.msg.attachments.at(x).download([str = ev.msg.attachments.at(x), &bot](const dpp::http_request_completion_t& k) {

                    if (k.error != dpp::http_error::h_success) {

                        printf_s("Error\n");
                        return;

                    }

                    else {
                        dpp::message msg(947975159053156352, "Uma imagem foi enviada.");
                        msg.add_file(str.filename, k.body);
                        bot.message_create(msg);
                    }

                });
            }
        }
    });
    bot.start(false);

    return 0;
}

