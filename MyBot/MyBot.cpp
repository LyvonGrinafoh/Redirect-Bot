#include <dpp/dpp.h>
#include <stdlib.h>
#include <stdio.h>
#include <dpp/message.h>

const std::string BOT_TOKEN = dpp::utility::read_file("Token.txt");
const dpp::snowflake MY_GUILD_ID = 907527594096853002;
const dpp::snowflake CHANNEL_1_ID = 806724162369355826;
const dpp::snowflake CHANNEL_2_ID = 790956999323025438;

int main()
{

    dpp::cluster bot(BOT_TOKEN, dpp::intents::i_default_intents | dpp::intents::i_guild_messages | dpp::intents::i_all_intents);

    bot.on_log(dpp::utility::cout_logger());

    bot.on_message_create([&](const dpp::message_create_t& ev) {

        if (ev.msg.author.is_bot() == false && ev.msg.attachments.size() > 0) {


            if (ev.msg.channel_id == 806724162369355826) {
                for (size_t x = 0; x < ev.msg.attachments.size(); x++) {

                    ev.msg.attachments.at(x).download([str = ev.msg.attachments.at(x), &bot, cid = ev.msg.channel_id, mid = ev.msg.id, username = ev.msg.author.username, content = ev.msg.content](const dpp::http_request_completion_t& k) {

                        if (k.error != dpp::http_error::h_success) {

                            printf_s("Error\n");
                            return;

                        }
                        std::string title = "`" + username + "` enviou um arquivo!";
                        std::string description = "`Mensagem:`\n```" + content + "```Link para o download do arquivo: " + str.url;
                        dpp::embed embed = dpp::embed().
                            set_color(0x9900ff).
                            set_title(title).
                            set_description(description).
                            set_image(str.url.c_str()).
                            set_timestamp(time(0));;

                        bot.message_create(dpp::message(CHANNEL_1_ID, embed));
                        bot.message_create(dpp::message(CHANNEL_2_ID, embed));
                        bot.message_delete(mid, cid, [](const dpp::confirmation_callback_t& u) {

                            if (u.is_error()) {

                                dpp::error_info error = u.get_error();
                                printf_s("Código de erro: %s \n", error.message.c_str());

                            }

                        });

                    });
                }
            }
        }
    });
    bot.start(false);

    return 0;
}

