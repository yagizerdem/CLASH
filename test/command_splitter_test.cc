//
// Created by pennywise on 26/16/25.
//

#include <gtest/gtest.h>

#include "../src/lexer/commandSplitter.h"

TEST(CommandSplitterTest, Basic) {
    CommandSplitter splitter;

    std::string input = "echo 'c/c++ programming' ;  echo 'java programming' \n cat a.txt | wc -l | echo   > a.txt \n  "
                        "echo yagiz erdem";
    std::vector<std::variant<Command, Pipe>> stream = splitter.commandStream(input);

    ASSERT_EQ(stream.size(), 4);
    Command cmd1 = std::get<0>(stream[0]);
    ASSERT_EQ(cmd1.rawShellCommand, "echo 'c/c++ programming'");
    Command cmd2 = std::get<0>(stream[1]);
    ASSERT_EQ(cmd2.rawShellCommand, "echo 'java programming'");
    Pipe p1 = std::get<1>(stream[2]);
    ASSERT_EQ(p1.commands[0].rawShellCommand, "cat a.txt");
    ASSERT_EQ(p1.commands[1].rawShellCommand, "wc -l");
    ASSERT_EQ(p1.commands[2].rawShellCommand, "echo   > a.txt");
    Command cmd3 = std::get<0>(stream[3]);
    ASSERT_EQ(cmd3.rawShellCommand, "echo yagiz erdem");

}

TEST(CommandSplitterTest, QuotedSeparatorsIgnored) {
    CommandSplitter splitter;

    std::string input =
        "echo 'a;b|c' ; echo \"x|y;z\"";

    auto stream = splitter.commandStream(input);

    ASSERT_EQ(stream.size(), 2);

    Command cmd1 = std::get<Command>(stream[0]);
    ASSERT_EQ(cmd1.rawShellCommand, "echo 'a;b|c'");

    Command cmd2 = std::get<Command>(stream[1]);
    ASSERT_EQ(cmd2.rawShellCommand, "echo \"x|y;z\"");
}

TEST(CommandSplitterTest, NoEmptyCommands) {
    CommandSplitter splitter;

    std::string input = "   ;   \n ; \n   ";

    auto stream = splitter.commandStream(input);

    ASSERT_TRUE(stream.empty());
}

TEST(CommandSplitterTest, PipeChain) {
    CommandSplitter splitter;

    std::string input = "a | b | c | d";

    auto stream = splitter.commandStream(input);

    ASSERT_EQ(stream.size(), 1);

    Pipe p = std::get<Pipe>(stream[0]);
    ASSERT_EQ(p.commands.size(), 4);

    ASSERT_EQ(p.commands[0].rawShellCommand, "a");
    ASSERT_EQ(p.commands[1].rawShellCommand, "b");
    ASSERT_EQ(p.commands[2].rawShellCommand, "c");
    ASSERT_EQ(p.commands[3].rawShellCommand, "d");
}

TEST(CommandSplitterTest, MixedSeparators) {
    CommandSplitter splitter;

    std::string input =
        "echo a\n"
        "echo b ; echo c\n"
        "echo d";

    auto stream = splitter.commandStream(input);

    ASSERT_EQ(stream.size(), 4);

    ASSERT_EQ(std::get<Command>(stream[0]).rawShellCommand, "echo a");
    ASSERT_EQ(std::get<Command>(stream[1]).rawShellCommand, "echo b");
    ASSERT_EQ(std::get<Command>(stream[2]).rawShellCommand, "echo c");
    ASSERT_EQ(std::get<Command>(stream[3]).rawShellCommand, "echo d");
}

TEST(CommandSplitterTest, PipeWithQuotesAndRedirect) {
    CommandSplitter splitter;

    std::string input =
        "echo \"a | b\" | grep a > out.txt";

    auto stream = splitter.commandStream(input);

    ASSERT_EQ(stream.size(), 1);

    Pipe p = std::get<Pipe>(stream[0]);
    ASSERT_EQ(p.commands.size(), 2);

    ASSERT_EQ(p.commands[0].rawShellCommand, "echo \"a | b\"");
    ASSERT_EQ(p.commands[1].rawShellCommand, "grep a > out.txt");
}

TEST(CommandSplitterTest, NestedQuotes) {
    CommandSplitter splitter;

    std::string input = "echo 'He said \"hello|world\"' ; echo \"It's a 'test;case'\"";

    auto stream = splitter.commandStream(input);

    ASSERT_EQ(stream.size(), 2);
    ASSERT_EQ(std::get<Command>(stream[0]).rawShellCommand, "echo 'He said \"hello|world\"'");
    ASSERT_EQ(std::get<Command>(stream[1]).rawShellCommand, "echo \"It's a 'test;case'\"");
}

TEST(CommandSplitterTest, EscapedCharacters) {
    CommandSplitter splitter;

    std::string input = "echo \\; test ; echo \\| pipe ; echo \\n newline";

    auto stream = splitter.commandStream(input);

    ASSERT_EQ(stream.size(), 3);
    ASSERT_EQ(std::get<Command>(stream[0]).rawShellCommand, "echo \\; test");
    ASSERT_EQ(std::get<Command>(stream[1]).rawShellCommand, "echo \\| pipe");
    ASSERT_EQ(std::get<Command>(stream[2]).rawShellCommand, "echo \\n newline");
}

TEST(CommandSplitterTest, MultipleConsecutiveSeparators) {
    CommandSplitter splitter;

    std::string input = "echo a ;; echo b | | echo c \n\n\n echo d";

    auto stream = splitter.commandStream(input);

    // Should handle double separators gracefully (no empty commands)
    ASSERT_GT(stream.size(), 0);
}

TEST(CommandSplitterTest, TrailingSeparators) {
    CommandSplitter splitter;

    std::string input = "echo hello ; echo world ;\n";

    auto stream = splitter.commandStream(input);

    ASSERT_EQ(stream.size(), 2);
    ASSERT_EQ(std::get<Command>(stream[0]).rawShellCommand, "echo hello");
    ASSERT_EQ(std::get<Command>(stream[1]).rawShellCommand, "echo world");
}

TEST(CommandSplitterTest, LeadingSeparators) {
    CommandSplitter splitter;

    std::string input = "; \n | echo hello";

    auto stream = splitter.commandStream(input);

    ASSERT_GT(stream.size(), 0);
    // First valid command should be "echo hello"
}

TEST(CommandSplitterTest, UnclosedQuote) {
    try {
        CommandSplitter splitter;

        std::string input = "echo 'unclosed quote ; echo test";

        auto stream = splitter.commandStream(input);

        // Should handle unclosed quotes - either treat entire string as one command
        // or handle it as an error case
        ASSERT_GT(stream.size(), 0);
    }catch (std::exception& e) {
        ASSERT_EQ(e.what(), std::string("syntax error: unterminated single quote (') at position 32"));
    }
}

TEST(CommandSplitterTest, EmptyQuotes) {
    CommandSplitter splitter;

    std::string input = "echo '' ; echo \"\" ; echo test";

    auto stream = splitter.commandStream(input);

    ASSERT_EQ(stream.size(), 3);
    ASSERT_EQ(std::get<Command>(stream[0]).rawShellCommand, "echo ''");
    ASSERT_EQ(std::get<Command>(stream[1]).rawShellCommand, "echo \"\"");
    ASSERT_EQ(std::get<Command>(stream[2]).rawShellCommand, "echo test");
}

TEST(CommandSplitterTest, ComplexPipeWithMultipleRedirects) {
    CommandSplitter splitter;

    std::string input = "cat file.txt | grep 'pattern' > out.txt | sort | uniq >> final.txt";

    auto stream = splitter.commandStream(input);

    ASSERT_EQ(stream.size(), 1);
    Pipe p = std::get<Pipe>(stream[0]);
    ASSERT_EQ(p.commands.size(), 4);
    ASSERT_EQ(p.commands[0].rawShellCommand, "cat file.txt");
    ASSERT_EQ(p.commands[1].rawShellCommand, "grep 'pattern' > out.txt");
    ASSERT_EQ(p.commands[2].rawShellCommand, "sort");
    ASSERT_EQ(p.commands[3].rawShellCommand, "uniq >> final.txt");
}

TEST(CommandSplitterTest, WhitespaceVariations) {
    CommandSplitter splitter;

    std::string input = "echo a   ;   echo b\t|\techo c\n\necho d";

    auto stream = splitter.commandStream(input);

    ASSERT_EQ(stream.size(), 3);
    ASSERT_EQ(std::get<Command>(stream[0]).rawShellCommand, "echo a");

    Pipe p = std::get<Pipe>(stream[1]);
    ASSERT_EQ(p.commands.size(), 2);
    ASSERT_EQ(p.commands[0].rawShellCommand, "echo b");
    ASSERT_EQ(p.commands[1].rawShellCommand, "echo c");

    ASSERT_EQ(std::get<Command>(stream[2]).rawShellCommand, "echo d");
}
TEST(CommandSplitterTest, CommandWithSpecialCharacters) {
    CommandSplitter splitter;

    std::string input = "echo $PATH ; echo ~user ; echo `date` ; echo $(pwd)";

    auto stream = splitter.commandStream(input);

    ASSERT_EQ(stream.size(), 4);
    ASSERT_EQ(std::get<Command>(stream[0]).rawShellCommand, "echo $PATH");
    ASSERT_EQ(std::get<Command>(stream[1]).rawShellCommand, "echo ~user");
    ASSERT_EQ(std::get<Command>(stream[2]).rawShellCommand, "echo `date`");
    ASSERT_EQ(std::get<Command>(stream[3]).rawShellCommand, "echo $(pwd)");
}

TEST(CommandSplitterTest, VeryLongCommand) {
    CommandSplitter splitter;

    std::string longArg(1000, 'x');
    std::string input = "echo " + longArg + " ; echo short";

    auto stream = splitter.commandStream(input);

    ASSERT_EQ(stream.size(), 2);
    ASSERT_EQ(std::get<Command>(stream[0]).rawShellCommand, "echo " + longArg);
    ASSERT_EQ(std::get<Command>(stream[1]).rawShellCommand, "echo short");
}

TEST(CommandSplitterTest, OnlyWhitespace) {
    CommandSplitter splitter;

    std::string input = "   \t\n\n   \t  ";

    auto stream = splitter.commandStream(input);

    ASSERT_TRUE(stream.empty());
}

TEST(CommandSplitterTest, SingleCharacterCommands) {
    CommandSplitter splitter;

    std::string input = "a ; b | c | d ; e";

    auto stream = splitter.commandStream(input);

    ASSERT_EQ(stream.size(), 3);
    ASSERT_EQ(std::get<Command>(stream[0]).rawShellCommand, "a");

    Pipe p = std::get<Pipe>(stream[1]);
    ASSERT_EQ(p.commands.size(), 3);

    ASSERT_EQ(std::get<Command>(stream[2]).rawShellCommand, "e");
}

TEST(CommandSplitterTest, QuotesWithBackslashes) {
    CommandSplitter splitter;

    std::string input = "echo 'path\\to\\file' ; echo \"C:\\Program Files\\App\"";

    auto stream = splitter.commandStream(input);

    ASSERT_EQ(stream.size(), 2);
    ASSERT_EQ(std::get<Command>(stream[0]).rawShellCommand, "echo 'path\\to\\file'");
    ASSERT_EQ(std::get<Command>(stream[1]).rawShellCommand, "echo \"C:\\Program Files\\App\"");
}

TEST(CommandSplitterTest, MixedQuotesAndPipes) {
    CommandSplitter splitter;

    std::string input = "echo 'start' | grep 'a|b' | echo \"c;d\" | sort";

    auto stream = splitter.commandStream(input);

    ASSERT_EQ(stream.size(), 1);
    Pipe p = std::get<Pipe>(stream[0]);
    ASSERT_EQ(p.commands.size(), 4);
    ASSERT_EQ(p.commands[0].rawShellCommand, "echo 'start'");
    ASSERT_EQ(p.commands[1].rawShellCommand, "grep 'a|b'");
    ASSERT_EQ(p.commands[2].rawShellCommand, "echo \"c;d\"");
    ASSERT_EQ(p.commands[3].rawShellCommand, "sort");
}

TEST(CommandSplitterTest, BackticksInCommand) {
    CommandSplitter splitter;

    std::string input = "echo `ls -la` ; echo test";

    auto stream = splitter.commandStream(input);

    ASSERT_EQ(stream.size(), 2);
    ASSERT_EQ(std::get<Command>(stream[0]).rawShellCommand, "echo `ls -la`");
    ASSERT_EQ(std::get<Command>(stream[1]).rawShellCommand, "echo test");
}

TEST(CommandSplitterTest, MultipleRedirectOperators) {
    CommandSplitter splitter;

    std::string input = "cat < input.txt > output.txt 2>&1";

    auto stream = splitter.commandStream(input);

    ASSERT_EQ(stream.size(), 1);
    ASSERT_EQ(std::get<Command>(stream[0]).rawShellCommand, "cat < input.txt > output.txt 2>&1");
}

TEST(CommandSplitterTest, PipeAtEnd) {
    CommandSplitter splitter;

    std::string input = "echo hello | grep h |";

    auto stream = splitter.commandStream(input);

    // Should handle trailing pipe - either ignore it or include as incomplete pipe
    ASSERT_GT(stream.size(), 0);
}


TEST(CommandSplitterTest, UltraComplexMultiLine1) {
    CommandSplitter splitter;

    std::string input =
        "cat /etc/passwd | grep 'root' | awk '{print $1}' | sort | uniq > /tmp/users.txt\n"
        "echo \"Processing data with special chars: pipe|here and semi;colon\"\n"
        "\n"
        "find . -name '*.cpp' -o -name '*.h' | xargs grep -n 'TODO' | sed 's/:/: /g' | sort -t ':' -k2 >> todo_list.txt\n"
        "\n\n"
        "ps aux | grep 'python' | awk '{print $2}' | xargs kill -9 ; echo 'Done killing processes'\n"
        "cat large.log | grep 'ERROR' | cut -d' ' -f1-3 | sort | uniq -c | sort -rn | head -20 > errors.txt";

    auto stream = splitter.commandStream(input);

    ASSERT_GE(stream.size(), 5);

    // First command: cat | grep | awk | sort | uniq > file
    Pipe p1 = std::get<Pipe>(stream[0]);
    ASSERT_EQ(p1.commands.size(), 5);
    ASSERT_EQ(p1.commands[0].rawShellCommand, "cat /etc/passwd");
    ASSERT_EQ(p1.commands[1].rawShellCommand, "grep 'root'");
    ASSERT_EQ(p1.commands[4].rawShellCommand, "uniq > /tmp/users.txt");

    // Second command: simple echo with special chars in quotes
    Command cmd2 = std::get<Command>(stream[1]);
    ASSERT_EQ(cmd2.rawShellCommand, "echo \"Processing data with special chars: pipe|here and semi;colon\"");
}

TEST(CommandSplitterTest, UltraComplexMultiLine2) {
    CommandSplitter splitter;

    std::string input =
        "docker ps -a | grep 'exited' | awk '{print $1}' | xargs -r docker rm\n"
        "\n"
        "curl -X POST 'https://api.example.com/data' -H 'Content-Type: application/json' -d '{\"key\": \"value|with|pipes\", \"data\": \"test;semicolon\"}' | jq '.result' | tee api.json\n"
        "\n\n\n"
        "echo \"Starting backup...\" ; tar -czf backup_$(date +%Y%m%d).tar.gz --exclude='*.log' --exclude='node_modules' ./ 2>&1 | tee backup.log ; echo \"Backup complete\"\n"
        "\n"
        "git log --all --pretty=format:'%h|%an|%ae|%s' | grep -v 'Merge' | awk -F'|' '{print $2}' | sort | uniq -c | sort -rn | head -10 > top_contributors.txt";

    auto stream = splitter.commandStream(input);

    ASSERT_GE(stream.size(), 4);
}

TEST(CommandSplitterTest, UltraComplexMultiLine3) {
    CommandSplitter splitter;

    std::string input =
        "netstat -tuln | grep 'LISTEN' | awk '{print $4}' | sed 's/.*://g' | sort -n | uniq > listening_ports.txt\n"
        "\n"
        "df -h | grep -E '^/dev/' | awk '{if($5+0 > 80) print $0}' | tee disk_alert.txt | wc -l\n"
        "\n\n"
        "echo 'System check started' ; free -m | grep 'Mem:' | awk '{print \"Used: \" $3 \"MB / \" $2 \"MB\"}' ; echo 'Memory check done'\n"
        "\n"
        "cat /var/log/syslog | grep 'error\\|warning\\|critical' | tail -100 | awk '{print $1, $2, $3, $5}' | sort | uniq -c | sort -rn > critical_events.txt\n"
        "vmstat 1 5 | tail -4 | awk '{sum+=$15} END {print \"Average CPU idle: \" sum/4 \"%\"}'";

    auto stream = splitter.commandStream(input);

    ASSERT_GE(stream.size(), 5);
}

TEST(CommandSplitterTest, UltraComplexMultiLine4) {
    CommandSplitter splitter;

    std::string input =
        "cat access.log | awk '{print $1}' | sort | uniq -c | sort -rn | head -50 > top_ips.txt ; "
        "cat access.log | awk '{print $7}' | sort | uniq -c | sort -rn | head -30 > top_urls.txt\n"
        "\n\n"
        "echo \"Analyzing logs with 'quotes' and \\\"escaped quotes\\\" and pipe|chars and semi;colons\"\n"
        "\n"
        "journalctl -u nginx.service --since '2 hours ago' | grep -i 'error' | awk '{print $5, $6, $7, $8, $9, $10}' | sort | uniq -c | sort -rn | head -20 > nginx_errors.txt\n"
        "journalctl -u mysql.service --since '1 hour ago' | grep -E 'slow query|deadlock' | wc -l | xargs -I {} echo \"MySQL issues found: {}\" > mysql_report.txt";

    auto stream = splitter.commandStream(input);

    ASSERT_GE(stream.size(), 4);
}

TEST(CommandSplitterTest, UltraComplexMultiLine5) {
    CommandSplitter splitter;

    std::string input =
        "python3 -c 'import sys; print(\"|\".join(sys.path))' | tr '|' '\\n' | grep -v '^$' | sort\n"
        "\n"
        "echo '{\"users\": [{\"name\": \"John|Doe\", \"age\": 30, \"email\": \"john@test.com\"}, {\"name\": \"Jane;Smith\", \"age\": 25, \"email\": \"jane@test.com\"}]}' | jq -r '.users[] | [.name, .age, .email] | @csv' | sort -t',' -k2 -n > sorted_users.csv\n"
        "\n\n\n"
        "ls -la | awk 'NR>3 {print $9, $5}' | sort -k2 -n | tail -10 | awk '{printf \"%-30s %10s bytes\\n\", $1, $2}'\n"
        "\n"
        "tcpdump -i eth0 -c 1000 -nn 'port 80 or port 443' 2>&1 | grep -oE '([0-9]{1,3}\\.){3}[0-9]{1,3}' | sort | uniq -c | sort -rn | head -20 > network_traffic.txt ; echo 'Traffic analysis complete'";

    auto stream = splitter.commandStream(input);

    ASSERT_GE(stream.size(), 4);
}

TEST(CommandSplitterTest, UltraComplexMultiLine6) {
    CommandSplitter splitter;

    std::string input =
        "cat large_data.csv | awk -F',' 'NR>1 {print $1\"|\"$3\"|\"$5\"|\"$7}' | grep -v '^$' | sort -t'|' -k2,2 -k3,3n | head -5000 | tail -1000 > filtered_data.txt\n"
        "\n\n"
        "echo 'Starting complex data processing pipeline...'\n"
        "cat data1.txt | grep 'pattern' | cut -d'|' -f1,3,5 > temp1.txt ; cat data2.txt | grep 'pattern' | cut -d';' -f2,4,6 > temp2.txt\n"
        "\n"
        "cat temp1.txt temp2.txt | sort | uniq | awk '{print NR, $0}' | tee combined.txt | wc -l | xargs -I {} echo \"Total unique records: {}\"\n"
        "\n\n\n"
        "du -sh ./* | sort -hr | head -15 | awk '{printf \"%-50s %10s\\n\", $2, $1}' | tee disk_usage.txt";

    auto stream = splitter.commandStream(input);

    ASSERT_GE(stream.size(), 5);
}

TEST(CommandSplitterTest, UltraComplexMultiLine7) {
    CommandSplitter splitter;

    std::string input =
        "echo \"Testing nested 'single' and \\\"double\\\" quotes with | pipes and ; semicolons inside\"\n"
        "printf '%s\\n' 'Line 1 | with | multiple | pipes' \"Line 2 ; with ; multiple ; semicolons\" 'Line 3 with \\'escaped\\' quotes' | grep -E '(pipe|semi|quote)' | sort | uniq\n"
        "\n\n"
        "cat /proc/cpuinfo | grep 'processor' | wc -l | xargs -I {} echo \"CPU cores: {}\" ; cat /proc/meminfo | grep 'MemTotal' | awk '{print \"RAM: \" $2/1024/1024 \" GB\"}'\n"
        "\n"
        "awk 'BEGIN {FS=\"|\"; OFS=\";\"} {print $1, $2, $3}' input.csv | sed 's/\"//g' | tr ';' '\\n' | sort | uniq -c > output.txt\n"
        "\n\n\n"
        "echo 'Final command with complex \"nested strings | and ; separators\"' | base64 | base64 -d";

    auto stream = splitter.commandStream(input);

    ASSERT_GE(stream.size(), 5);
}

TEST(CommandSplitterTest, UltraComplexMultiLine8) {
    CommandSplitter splitter;

    std::string input =
        "tail -n 10000 /var/log/syslog | grep --line-buffered 'ERROR\\|WARNING\\|CRITICAL' | awk '{print strftime(\"%Y-%m-%d %H:%M:%S\"), $0}' | tee -a filtered_syslog.log | wc -l\n"
        "\n"
        "zcat access.log.*.gz | awk '$9 >= 500 {print $1, $7, $9}' | sort | uniq -c | sort -rn | head -50 | awk '{printf \"Count: %-6s IP: %-15s URL: %-50s Status: %s\\n\", $1, $2, $3, $4}' > server_errors.txt\n"
        "\n\n\n"
        "echo 'Starting comprehensive system diagnostics...' ; uptime | awk '{print \"Load average: \" $(NF-2), $(NF-1), $NF}' ; free -h | grep 'Mem:' ; df -h | grep -E '^/dev/'\n"
        "\n"
        "ps aux | awk '{sum+=$3; print $0} END {print \"Total CPU usage: \" sum \"%\"}' | tail -20 | tee process_report.txt ; echo 'Diagnostics complete'";

    auto stream = splitter.commandStream(input);

    ASSERT_GE(stream.size(), 3);
}

TEST(CommandSplitterTest, UltraComplexMultiLine9) {
    CommandSplitter splitter;

    std::string input =
        "cat data.json | jq -r '.items[] | [.id, .name, .status] | @tsv' | awk -F'\\t' '{print $1\"|\"$2\"|\"$3}' | grep 'active' | sort -t'|' -k2 > active_items.txt\n"
        "\n\n"
        "echo \"Multi-line test with\\n'quotes|pipes;semicolons' and \\\"more\\tnesting\\\" here\" ; echo 'Second part with $(command) substitution chars'\n"
        "\n"
        "find . -type f -name '*.log' -mtime +7 | xargs -I {} sh -c 'gzip \"{}\" && echo \"Compressed: {}\"' | tee compression.log | wc -l | xargs -I {} echo \"Total files compressed: {}\"\n"
        "\n\n\n"
        "cat /etc/services | grep -E '^[a-z]' | awk '{print $2}' | cut -d'/' -f1 | sort -n | uniq | tail -100 | head -10 > common_ports.txt\n"
        "lsof -i -P -n | grep LISTEN | awk '{print $9}' | cut -d':' -f2 | sort -n | uniq | comm -12 - common_ports.txt | tee active_common_ports.txt";

    auto stream = splitter.commandStream(input);

    ASSERT_GE(stream.size(), 4);
}

TEST(CommandSplitterTest, UltraComplexMultiLine10) {
    CommandSplitter splitter;

    std::string input =
        "echo 'Step 1: Extract and transform' ; cat raw_data.txt | tr '[:lower:]' '[:upper:]' | sed 's/[^A-Z|;]//g' | grep -v '^$' > step1.txt\n"
        "\n"
        "echo 'Step 2: Filter and sort' ; cat step1.txt | sort | uniq -c | sort -rn | head -100 | awk '{print $2, $1}' > step2.txt\n"
        "\n\n\n"
        "echo 'Step 3: Complex pipeline with \"special|chars\" and \\'quotes\\'' | tee step3_start.log\n"
        "cat step2.txt | awk '{sum+=$2; print $0, sum}' | tail -50 | awk '{print $1\"|\"$2\"|\"$3}' | sed 's/|/ | /g' | tee step3.txt | wc -l\n"
        "\n"
        "echo 'Step 4: Final aggregation' ; cat step3.txt | cut -d'|' -f1,2 | sort -t'|' -k2 -n | tail -20 | awk -F'|' '{printf \"Item: %-20s Count: %10s\\n\", $1, $2}' | tee final_report.txt\n"
        "\n\n"
        "cat final_report.txt | wc -l | xargs -I {} echo \"Report generated with {} entries\" ; echo 'Pipeline complete!'";

    auto stream = splitter.commandStream(input);

    ASSERT_GE(stream.size(), 7);
}

TEST(CommandSplitterTest, CrazyNestedQuotesMultiLine) {
    CommandSplitter splitter;

    std::string input =
        "echo \"Level 1: 'Level 2: \\\"Level 3 | pipe ; semi\\\" back to 2' back to 1\"\n"
        "\n"
        "printf '%s\\n' 'String with | pipes' \"String with ; semicolons\" 'String with \\'escaped\\' quotes' \"String with \\\"escaped\\\" quotes\" | grep -E '(pipe|semi|escape)'\n"
        "\n\n"
        "echo 'Command with $(echo \"nested|command\") and `echo \"backtick;command\"` substitutions' | sed 's/nested/NESTED/g' | sed 's/backtick/BACKTICK/g'\n"
        "\n"
        "awk 'BEGIN {FS=\"|\"; OFS=\";\"} {for(i=1;i<=NF;i++) print i, $i}' data.txt | grep -v '^$' | head -20 > awk_output.txt";

    auto stream = splitter.commandStream(input);

    ASSERT_GE(stream.size(), 4);
}

TEST(CommandSplitterTest, MassiveRealWorldScenario) {
    CommandSplitter splitter;

    std::string input =
        "echo '=== Starting comprehensive log analysis pipeline ===' | tee analysis.log\n"
        "\n"
        "cat /var/log/nginx/access.log | awk '{print $1}' | sort | uniq -c | sort -rn | head -100 | awk '{printf \"%-20s %10s requests\\n\", $2, $1}' | tee top_ips.txt\n"
        "\n\n"
        "cat /var/log/nginx/access.log | awk '{print $7}' | grep -E '\\.(jpg|png|gif|css|js)$' | sort | uniq -c | sort -rn | head -50 > static_files.txt\n"
        "cat /var/log/nginx/access.log | awk '{print $7}' | grep -v -E '\\.(jpg|png|gif|css|js)$' | sort | uniq -c | sort -rn | head -50 > dynamic_pages.txt\n"
        "\n\n\n"
        "echo 'Analyzing error rates...' ; cat /var/log/nginx/access.log | awk '{print $9}' | sort | uniq -c | awk '{print $2, $1}' | sort -n | awk '{printf \"Status %-4s: %8s occurrences\\n\", $1, $2}' | tee status_codes.txt\n"
        "\n"
        "cat /var/log/nginx/error.log | grep -E 'error|warn|crit' | awk '{print $4}' | tr -d '[]' | sort | uniq -c | sort -rn | head -20 | awk '{print $2, $1}' > error_levels.txt\n"
        "\n\n"
        "echo \"Analysis complete at $(date '+%Y-%m-%d %H:%M:%S')\" | tee -a analysis.log ; cat analysis.log top_ips.txt status_codes.txt | head -50 > summary_report.txt\n"
        "echo '=== Pipeline finished ==='";

    auto stream = splitter.commandStream(input);

    ASSERT_GE(stream.size(), 8);
}