import std;

#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <poll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

namespace detail {

class UniqueFd {
public:
    UniqueFd() noexcept = default;
    explicit UniqueFd(const int fd) noexcept : fd_(fd) {}
    ~UniqueFd() noexcept { reset(); }

    UniqueFd(const UniqueFd&) = delete;
    UniqueFd& operator=(const UniqueFd&) = delete;

    UniqueFd(UniqueFd&& other) noexcept : fd_(std::exchange(other.fd_, -1)) {}
    UniqueFd& operator=(UniqueFd&& other) noexcept {
        if (this != &other) {
            reset(std::exchange(other.fd_, -1));
        }
        return *this;
    }

    [[nodiscard]] int get() const noexcept { return fd_; }
    [[nodiscard]] explicit operator bool() const noexcept { return fd_ >= 0; }

    void reset(const int new_fd = -1) noexcept {
        if (fd_ >= 0) {
            ::close(fd_);
        }
        fd_ = new_fd;
    }

private:
    int fd_{-1};
};

struct ThoughtInfo {
    std::string sigil;
    std::string type;
    std::string flavor;
    std::string verb;
    std::string form{"kebab"};
    std::string action;
};

struct OptionInfo {
    int index{0};
    std::string skill;
    int difficulty{0};
    std::string text;
    std::string messy;
    std::string crit;
};

struct FactRecord {
    std::string id;
    std::string kind;
    std::string provenance;
    std::map<std::string, std::string, std::less<>> attrs;
    std::string raw_line;
};

struct ClaimRecord {
    std::string id;
    int suspect{0};
    std::string name;
    std::string at;
};

struct ParsedTag {
    std::string tag;
    std::map<std::string, std::string, std::less<>> attrs;
};

[[nodiscard]] constexpr std::string_view trim_sv(std::string_view sv) noexcept {
    while (!sv.empty() && std::isspace(static_cast<unsigned char>(sv.front()))) {
        sv.remove_prefix(1);
    }
    while (!sv.empty() && std::isspace(static_cast<unsigned char>(sv.back()))) {
        sv.remove_suffix(1);
    }
    return sv;
}

[[nodiscard]] std::optional<ParsedTag> parse_protocol_line(std::string_view line) {
    if (!line.starts_with("!!")) {
        return std::nullopt;
    }
    line.remove_prefix(2);
    line = trim_sv(line);
    if (line.empty()) {
        return std::nullopt;
    }

    ParsedTag result;
    const auto space_pos = line.find(' ');
    if (space_pos == std::string_view::npos) {
        result.tag = std::string(line);
        return result;
    }

    result.tag = std::string(line.substr(0, space_pos));
    line = trim_sv(line.substr(space_pos + 1));

    while (!line.empty()) {
        const auto eq_pos = line.find('=');

        if (const auto next_space = line.find(' ');
            eq_pos == std::string_view::npos || (next_space != std::string_view::npos && next_space < eq_pos)) {
            std::string flag_token(line.substr(0, next_space));
            result.attrs.emplace(std::move(flag_token), "1");
            if (next_space == std::string_view::npos) {
                break;
            }
            line = trim_sv(line.substr(next_space + 1));
            continue;
        }

        std::string key(trim_sv(line.substr(0, eq_pos)));
        line.remove_prefix(eq_pos + 1);

        std::string val;
        if (!line.empty() && line.front() == '"') {
            line.remove_prefix(1);
            if (const auto end_quote = line.find('"');
                end_quote == std::string_view::npos) {
                val = std::string(line);
                line = {};
            } else {
                val = std::string(line.substr(0, end_quote));
                line.remove_prefix(end_quote + 1);
            }
        } else {
            const auto sp = line.find(' ');
            if (sp == std::string_view::npos) {
                val = std::string(line);
                line = {};
            } else {
                val = std::string(line.substr(0, sp));
                line.remove_prefix(sp + 1);
            }
        }
        result.attrs.emplace(std::move(key), std::move(val));
        line = trim_sv(line);
    }

    return result;
}

[[nodiscard]] std::string format_urge(const std::string_view action, const std::string_view form) {
    std::vector<std::string> words;
    std::string current;

    for (const char ch : action) {
        if (std::isalnum(static_cast<unsigned char>(ch))) {
            current.push_back(ch);
        } else if (!current.empty()) {
            words.push_back(std::move(current));
            current.clear();
        }
    }
    if (!current.empty()) {
        words.push_back(std::move(current));
    }

    if (form == "reversed") {
        std::ranges::reverse(words);
    } else if (form == "shout") {
        for (auto& w : words) {
            for (char& c : w) {
                c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
            }
        }
    }

    std::string out;
    for (std::size_t i = 0; i < words.size(); ++i) {
        if (i > 0) {
            out.push_back('-');
        }
        out += words[i];
    }
    return out;
}

[[nodiscard]] std::optional<OptionInfo> parse_option_line(std::string_view line) {
    const auto close_paren = line.find(") [");
    if (close_paren == std::string_view::npos) {
        return std::nullopt;
    }

    OptionInfo opt;
    const auto idx_sv = trim_sv(line.substr(0, close_paren));
    if (std::from_chars(idx_sv.data(), idx_sv.data() + idx_sv.size(), opt.index).ec != std::errc{}) {
        return std::nullopt;
    }

    const auto bracket_start = close_paren + 3;
    const auto bracket_end = line.find(']', bracket_start);
    if (bracket_end == std::string_view::npos) {
        return std::nullopt;
    }

    auto inside_bracket = line.substr(bracket_start, bracket_end - bracket_start);
    const auto colon_pos = inside_bracket.find(':');
    if (colon_pos == std::string_view::npos) {
        return std::nullopt;
    }
    opt.skill = std::string(inside_bracket.substr(0, colon_pos));

    const auto last_space = inside_bracket.rfind(' ');
    if (last_space != std::string_view::npos) {
        const auto diff_sv = inside_bracket.substr(last_space + 1);
        std::from_chars(diff_sv.data(), diff_sv.data() + diff_sv.size(), opt.difficulty);
    }

    const auto q1 = line.find('"', bracket_end);
    const auto q2 = (q1 != std::string_view::npos) ? line.find('"', q1 + 1) : std::string_view::npos;
    if (q1 != std::string_view::npos && q2 != std::string_view::npos) {
        opt.text = std::string(line.substr(q1 + 1, q2 - q1 - 1));
    }

    auto extract_tag = [&](const std::string_view key) -> std::string {
        auto pos = line.find(key);
        if (pos == std::string_view::npos) {
            return {};
        }
        pos += key.size();
        const auto end = line.find(')', pos);
        if (end == std::string_view::npos) {
            return {};
        }
        return std::string(line.substr(pos, end - pos));
    };

    opt.messy = extract_tag("(messy: ");
    opt.crit  = extract_tag("(crit: ");
    return opt;
}

[[nodiscard]] std::string predict_clue_kind(const std::string_view text) {
    if (text.find("written statements") != std::string_view::npos) return "statements";
    if (text.find("counter first go below zero") != std::string_view::npos) return "deploy-time";
    if (text.find("define 'healthy'") != std::string_view::npos) return "deploy-time";
    if (text.find("Read me the handoff") != std::string_view::npos) return "deploy-time";
    if (text.find("Picture the change going out") != std::string_view::npos) return "deploy-path";
    if (text.find("who asked you to ship the last deploy") != std::string_view::npos) return "deploy-path";
    if (text.find("scroll back to the start of the shift") != std::string_view::npos) return "deploy-path";
    if (text.find("core dump from the checkout") != std::string_view::npos) return "change";
    if (text.find("release notes for the last deploy") != std::string_view::npos) return "change";
    if (text.find("deleted ticket") != std::string_view::npos) return "change";
    if (text.find("Who are you protecting") != std::string_view::npos) return "staffing";
    if (text.find("Who was working on what") != std::string_view::npos) return "staffing";
    if (text.find("founder's planner") != std::string_view::npos) return "staffing";
    if (text.find("coffee machine's log") != std::string_view::npos) return "badge-log";
    if (text.find("log of who left when") != std::string_view::npos) return "badge-log";
    if (text.find("carbon copy from under the handoff") != std::string_view::npos) return "badge-log";
    if (text.find("still in the building when the board") != std::string_view::npos) return "badge-log";
    if (text.find("Think like the founder") != std::string_view::npos) return "badge-map";
    if (text.find("hide the good coffee") != std::string_view::npos) return "badge-map";
    if (text.find("order HELM-9 to deploy from this channel") != std::string_view::npos) return "channel-admins";
    if (text.find("Pretend to be from compliance") != std::string_view::npos) return "channel-admins";
    if (text.find("access roster") != std::string_view::npos) return "key-holders";
    if (text.find("night-ops deploy key") != std::string_view::npos) return "key-holders";
    if (text.find("Search every drawer") != std::string_view::npos) return "key-holders / badge-map";
    if (text.find("last slept") != std::string_view::npos ||
        text.find("dead plant") != std::string_view::npos ||
        text.find("pizza order") != std::string_view::npos) {
        return "dead-end (avoid)";
    }
    return "lead / unlock";
}

[[nodiscard]] int parse_hhmm(std::string_view hhmm) noexcept {
    const auto colon = hhmm.find(':');
    if (colon == std::string_view::npos) {
        return -1;
    }
    int h = 0;
    int m = 0;
    const auto h_sv = trim_sv(hhmm.substr(0, colon));
    const auto m_sv = trim_sv(hhmm.substr(colon + 1));
    std::from_chars(h_sv.data(), h_sv.data() + h_sv.size(), h);
    std::from_chars(m_sv.data(), m_sv.data() + m_sv.size(), m);
    return h * 60 + m;
}

[[nodiscard]] std::vector<std::string> split_by(const std::string_view sv, const char delim) {
    std::vector<std::string> out;
    std::size_t start = 0;
    while (start <= sv.size()) {
        const auto pos = sv.find(delim, start);
        if (pos == std::string_view::npos) {
            out.emplace_back(trim_sv(sv.substr(start)));
            break;
        }
        out.emplace_back(trim_sv(sv.substr(start, pos - start)));
        start = pos + 1;
    }
    return out;
}

class DiscoSolver {
public:
    DiscoSolver(const int read_fd, const int write_fd)
        : read_fd_(read_fd), write_fd_(write_fd) {
        ritual_verbs_["COMPULSION"]    = "FORBID";
        ritual_verbs_["PARANOIA"]      = "REBUKE";
        ritual_verbs_["MORTIFICATION"] = "DISMISS";
        ritual_verbs_["EUPHORIA"]      = "REFUSE";
        ritual_verbs_["MALAISE"]       = "DEFY";
    }

    void send_cmd(std::string_view cmd) {
        const std::string packet = std::format("{}\n", cmd);
        const char* buf = packet.data();
        std::size_t remaining = packet.size();
        while (remaining > 0) {
            const ssize_t written = ::write(write_fd_, buf, remaining);
            if (written < 0) {
                if (errno == EINTR) {
                    continue;
                }
                break;
            }
            if (written == 0) {
                break;
            }
            buf += written;
            remaining -= static_cast<std::size_t>(written);
        }
        std::cout << std::format("\033[1;32m[BOT >>] {}\033[0m\n", cmd) << std::flush;
    }

    void process_server_line(std::string_view line) {
        if (line.starts_with("!!")) {
            std::cout << std::format("\033[0;36m{}\033[0m\n", line);
        } else if (line.starts_with("!!!")) {
            std::cout << std::format("\033[1;31m{}\033[0m\n", line);
        } else {
            std::cout << line << '\n';
        }

        if (reading_options_) {
            if (auto opt = parse_option_line(line)) {
                current_options_.push_back(std::move(*opt));
                return;
            }
        }

        auto parsed_opt = parse_protocol_line(line);
        if (!parsed_opt) {
            return;
        }
        const auto& [tag, attrs] = *parsed_opt;

        if (tag == "SHEET") {
            skills_.clear();
            facts_.clear();
            claims_.clear();
            suspects_.clear();
            verified_kinds_.clear();
            has_statements_ = false;
            for (const auto& [k, v] : attrs) {
                int val = 0;
                std::from_chars(v.data(), v.data() + v.size(), val);
                skills_[k] = val;
            }
        } else if (tag == "SUSPECT") {
            if (attrs.contains("n") && attrs.contains("name")) {
                int n = 0;
                std::from_chars(attrs.at("n").data(),
                                attrs.at("n").data() + attrs.at("n").size(),
                                n);
                suspects_[n] = attrs.at("name");
            }
        } else if (tag == "RITUAL") {
            if (attrs.contains("flavor") && attrs.contains("verb")) {
                ritual_verbs_[attrs.at("flavor")] = attrs.at("verb");
            }
        } else if (tag == "MIND") {
            if (attrs.contains("count")) {
                std::from_chars(attrs.at("count").data(),
                                attrs.at("count").data() + attrs.at("count").size(),
                                mind_count_);
            }
            if (attrs.contains("limit")) {
                std::from_chars(attrs.at("limit").data(),
                                attrs.at("limit").data() + attrs.at("limit").size(),
                                mind_limit_);
            }
        } else if (tag == "THOUGHT-ATTACH") {
            ThoughtInfo info;
            if (attrs.contains("sigil"))  info.sigil  = attrs.at("sigil");
            if (attrs.contains("type"))   info.type   = attrs.at("type");
            if (attrs.contains("flavor")) info.flavor = attrs.at("flavor");
            if (attrs.contains("action")) info.action = attrs.at("action");
            if (!info.sigil.empty()) {
                attached_thoughts_[info.sigil] = std::move(info);
            }
        } else if (tag == "THOUGHT-STRIKE") {
            handle_thought_strike(attrs);
        } else if (tag == "WARD-WINDOW") {
            int count = 1;
            if (attrs.contains("count")) {
                std::from_chars(attrs.at("count").data(),
                                attrs.at("count").data() + attrs.at("count").size(),
                                count);
            }
            flush_wards(count);
        } else if (tag == "SCENE") {
            current_scene_ = attrs.contains("id") ? attrs.at("id") : "";
            current_options_.clear();
        } else if (tag == "OPTIONS") {
            current_options_.clear();
            reading_options_ = true;
        } else if (tag == "PICK") {
            reading_options_ = false;
            print_options_analysis();
        } else if (tag == "FACT") {
            FactRecord rec;
            rec.id         = attrs.contains("id") ? attrs.at("id") : "";
            rec.kind       = attrs.contains("kind") ? attrs.at("kind") : "";
            rec.provenance = attrs.contains("provenance") ? attrs.at("provenance") : "";
            rec.attrs      = attrs;
            rec.raw_line   = std::string(line);
            if (rec.provenance == "verified" && !rec.kind.empty()) {
                verified_kinds_.insert(rec.kind);
            }
            facts_.push_back(std::move(rec));
        } else if (tag == "STATEMENTS") {
            if (attrs.contains("earned") && attrs.at("earned") == "1") {
                has_statements_ = true;
                verified_kinds_.insert("statements");
            }
        } else if (tag == "CLAIM") {
            ClaimRecord cl;
            cl.id   = attrs.contains("id") ? attrs.at("id") : "";
            cl.name = attrs.contains("name") ? attrs.at("name") : "";
            cl.at   = attrs.contains("at") ? attrs.at("at") : "";
            if (attrs.contains("suspect")) {
                std::from_chars(attrs.at("suspect").data(),
                                attrs.at("suspect").data() + attrs.at("suspect").size(),
                                cl.suspect);
            }
            if (cl.suspect > 0) {
                claims_[cl.suspect] = std::move(cl);
            }
        } else if (tag == "PROMPT" && attrs.contains("accuse")) {
            solve_and_print_review();
        }
    }

    void handle_user_input(const std::string_view input) {
        if (input == "!status") {
            print_status();
            solve_and_print_review();
            return;
        }
        send_cmd(input);
    }

private:
    void handle_thought_strike(const std::map<std::string, std::string, std::less<>>& attrs) {
        std::string sigil  = attrs.contains("sigil") ? attrs.at("sigil") : "";
        const ThoughtInfo mem    = attached_thoughts_.contains(sigil) ? attached_thoughts_.at(sigil) : ThoughtInfo{};
        const std::string flavor = attrs.contains("flavor") ? attrs.at("flavor") : mem.flavor;
        std::string verb   = attrs.contains("verb")   ? attrs.at("verb")
                           : (ritual_verbs_.contains(flavor) ? ritual_verbs_.at(flavor) : "");
        const std::string form   = attrs.contains("form")   ? attrs.at("form")   : "kebab";
        const std::string action = attrs.contains("action") ? attrs.at("action") : mem.action;

        std::string formed_urge = format_urge(action, form);
        pending_wards_.push_back(std::format("WARD {} {} {}", sigil, verb, formed_urge));
    }

    void flush_wards(const int expected_count) {
        if (pending_wards_.empty()) {
            return;
        }
        while (std::cmp_less(pending_wards_.size(), expected_count)) {
            pending_wards_.push_back(pending_wards_.back());
        }
        for (const auto& cmd : pending_wards_) {
            send_cmd(cmd);
        }
        pending_wards_.clear();
    }

    void print_options_analysis() const {
        if (current_options_.empty()) {
            return;
        }

        std::cout << "\n\033[1;35mCollected clues: ";
        if (verified_kinds_.empty()) {
            std::cout << "(none)";
        } else {
            for (const auto& k : verified_kinds_) {
                std::cout << "[" << k << "] ";
            }
        }
        std::cout << "\033[0m\n";

        std::cout << "\033[1;33m=== OPTIONS ANALYSIS [" << current_scene_
                  << "] (Mind: " << mind_count_ << "/" << mind_limit_ << ") ===\033[0m\n";

        for (const auto& opt : current_options_) {
            int sk_val    = skills_.contains(opt.skill) ? skills_.at(opt.skill) : 0;
            int max_reach = sk_val + 5;
            int diff      = opt.difficulty;
            int margin    = max_reach - diff;

            std::string clue = predict_clue_kind(opt.text);
            const bool already_have = verified_kinds_.contains(clue);

            std::string status_str;
            if (margin >= 0) {
                status_str = std::format("\033[1;32m[PASS: 100% SUCCESS{}]\033[0m",
                                         opt.messy == "-" ? " (NO PENALTY)" : "");
            } else if (margin >= -2) {
                if (mind_count_ + 2 < mind_limit_) {
                    status_str = std::format("\033[1;36m[SHORT {}: SEND PAY (+2 thoughts)]\033[0m", margin);
                } else {
                    status_str = std::format("\033[1;31m[SHORT {}: CANNOT PAY (Mind={}/5)]\033[0m",
                                             margin, mind_count_);
                }
            } else {
                status_str = std::format("\033[1;31m[FAIL {}: AVOID]\033[0m", margin);
            }

            std::string clue_str = already_have
                ? std::format("\033[1;31m-> CLUE: [{}] (DUPLICATE)\033[0m", clue)
                : std::format("\033[1;33m-> CLUE: [{}]\033[0m", clue);

            std::cout << std::format("  {}) [{}: {} (skill {}+5={})] {}\n     {} {}\n",
                                     opt.index, opt.skill, diff, sk_val, max_reach,
                                     opt.text, status_str, clue_str);
        }
        std::cout << "\033[1;33m------------------------------------------------------------\033[0m\n" << std::flush;
    }

    [[nodiscard]] const FactRecord* find_verified(const std::string_view kind) const {
        for (const auto& f : facts_) {
            if (f.provenance == "verified" && f.kind == kind) {
                return &f;
            }
        }
        return nullptr;
    }

    void solve_and_print_review() const {
        struct StrandResult {
            std::string name;
            std::set<int> allowed_suspects;
            std::vector<std::string> fact_ids;
        };

        std::vector<StrandResult> strands;

        auto match_suspect_id = [&](const std::string_view name_sv) -> int {
            for (const auto& [id, sname] : suspects_) {
                if (sname == name_sv || sname.find(name_sv) != std::string::npos ||
                    name_sv.find(sname) != std::string::npos) {
                    return id;
                }
            }
            return 0;
        };

        const auto* f_time = find_verified("deploy-time");
        const auto* f_blog = find_verified("badge-log");
        const auto* f_bmap = find_verified("badge-map");

        std::map<int, int> suspect_badge_out;
        if (f_blog && f_bmap && f_blog->attrs.contains("entries") && f_bmap->attrs.contains("entries")) {
            std::map<std::string, int, std::less<>> badge_to_time;
            for (const auto& item : split_by(f_blog->attrs.at("entries"), ';')) {
                auto eq = item.find('=');
                if (eq != std::string::npos) {
                    badge_to_time[item.substr(0, eq)] = parse_hhmm(item.substr(eq + 1));
                }
            }
            for (const auto& item : split_by(f_bmap->attrs.at("entries"), ';')) {
                auto eq = item.find('=');
                if (eq != std::string::npos) {
                    std::string badge = item.substr(0, eq);
                    std::string sname = item.substr(eq + 1);
                    int sid = match_suspect_id(sname);
                    if (sid > 0 && badge_to_time.contains(badge)) {
                        suspect_badge_out[sid] = badge_to_time.at(badge);
                    }
                }
            }
        }

        if (f_time && f_blog && f_bmap && f_time->attrs.contains("at") && !suspect_badge_out.empty()) {
            int dep_mins = parse_hhmm(f_time->attrs.at("at"));
            StrandResult sr{"when", {}, {f_time->id, f_blog->id, f_bmap->id}};
            for (const auto& [sid, out_mins] : suspect_badge_out) {
                if (out_mins >= dep_mins) {
                    sr.allowed_suspects.insert(sid);
                }
            }
            strands.push_back(std::move(sr));
        }

        if (has_statements_ && f_blog && f_bmap && !suspect_badge_out.empty() && !claims_.empty()) {
            StrandResult sr{"lie", {}, {f_blog->id, f_bmap->id}};
            for (const auto& [sid, cl] : claims_) {
                if (suspect_badge_out.contains(sid)) {
                    int claim_mins = parse_hhmm(cl.at);
                    int out_mins   = suspect_badge_out.at(sid);
                    if (out_mins - claim_mins > 5) {
                        sr.allowed_suspects.insert(sid);
                    }
                }
            }
            strands.push_back(std::move(sr));
        }

        const auto* f_change   = find_verified("change");
        const auto* f_staffing = find_verified("staffing");
        if (f_change && f_staffing && f_change->attrs.contains("stream") && f_staffing->attrs.contains("entries")) {
            std::string target_stream = f_change->attrs.at("stream");
            StrandResult sr{"what", {}, {f_change->id, f_staffing->id}};
            for (const auto& group : split_by(f_staffing->attrs.at("entries"), ';')) {
                auto eq = group.find('=');
                if (eq != std::string::npos && group.substr(0, eq) == target_stream) {
                    for (const auto& person : split_by(group.substr(eq + 1), '|')) {
                        if (int sid = match_suspect_id(person); sid > 0) {
                            sr.allowed_suspects.insert(sid);
                        }
                    }
                }
            }
            strands.push_back(std::move(sr));
        }

        const auto* f_path = find_verified("deploy-path");
        if (f_path && f_path->attrs.contains("path")) {
            std::string path_kind = (f_path->attrs.at("path") == "key") ? "key-holders" : "channel-admins";
            const auto* f_holders = find_verified(path_kind);
            if (f_holders && f_holders->attrs.contains("people")) {
                StrandResult sr{"how", {}, {f_path->id, f_holders->id}};
                for (const auto& person : split_by(f_holders->attrs.at("people"), '|')) {
                    if (int sid = match_suspect_id(person); sid > 0) {
                        sr.allowed_suspects.insert(sid);
                    }
                }
                strands.push_back(std::move(sr));
            }
        }

        std::cout << "\n\033[1;35m================ CASE DEDUCTION (REVIEW) ================\033[0m\n";
        for (const auto& sr : strands) {
            std::cout << std::format("  Strand [{}]: leaves suspects {{ ", sr.name);
            for (int s : sr.allowed_suspects) {
                std::cout << s << " ";
            }
            std::cout << "} (facts: ";
            for (const auto& fid : sr.fact_ids) {
                std::cout << fid << " ";
            }
            std::cout << ")\n";
        }

        int best_suspect = 0;
        std::vector<std::string> best_facts;
        bool uses_lie = false;

        std::size_t m = strands.size();
        for (std::size_t mask = 1; mask < (1ULL << m); ++mask) {
            std::set<int> inter{1, 2, 3, 4};
            std::vector<std::string> fids;
            bool has_lie = false;

            for (std::size_t i = 0; i < m; ++i) {
                if (mask & (1ULL << i)) {
                    std::set<int> next_inter;
                    std::ranges::set_intersection(inter, strands[i].allowed_suspects,
                                                  std::inserter(next_inter, next_inter.begin()));
                    inter = std::move(next_inter);
                    if (strands[i].name == "lie") {
                        has_lie = true;
                    }
                    for (const auto& fid : strands[i].fact_ids) {
                        if (!std::ranges::contains(fids, fid)) {
                            fids.push_back(fid);
                        }
                    }
                }
            }

            if (inter.size() == 1) {
                if (best_suspect == 0 || fids.size() < best_facts.size()) {
                    best_suspect = *inter.begin();
                    best_facts   = std::move(fids);
                    uses_lie     = has_lie;
                }
            }
        }

        if (best_suspect > 0) {
            std::string proof_cmd = "PROOF";
            for (const auto& fid : best_facts) {
                proof_cmd += " " + fid;
            }
            std::string claim_id = claims_.contains(best_suspect) ? claims_.at(best_suspect).id : "";

            std::cout << std::format(
                "\033[1;32m>>> CULPRIT IDENTIFIED: #{} - {}\033[0m\n"
                "\033[1;32m>>> Step 1. Send:  ACCUSE {}\033[0m\n"
                "\033[1;32m>>> Step 2. Send:  {}{}\033[0m\n",
                best_suspect,
                suspects_.contains(best_suspect) ? suspects_.at(best_suspect) : "",
                best_suspect,
                proof_cmd,
                (uses_lie && !claim_id.empty())
                    ? std::format("   (or: {} {})", proof_cmd, claim_id)
                    : "");
        } else {
            std::cout << "\033[1;31m>>> Insufficient complete strands to isolate one suspect (send WALK)\033[0m\n";
        }
        std::cout << "\033[1;35m=========================================================\033[0m\n" << std::flush;
    }

    void print_status() const {
        std::cout << "\033[1;35m=== COLLECTED FACTS ===\033[0m\n";
        for (const auto& f : facts_) {
            std::cout << "  " << f.raw_line << '\n';
        }
    }

    int read_fd_{-1};
    int write_fd_{-1};

    std::map<std::string, int, std::less<>> skills_;
    std::map<int, std::string> suspects_;
    std::map<std::string, std::string, std::less<>> ritual_verbs_;
    std::map<std::string, ThoughtInfo, std::less<>> attached_thoughts_;
    std::vector<std::string> pending_wards_;

    int mind_count_{0};
    int mind_limit_{5};
    std::string current_scene_;
    bool reading_options_{false};
    std::vector<OptionInfo> current_options_;

    std::vector<FactRecord> facts_;
    std::set<std::string, std::less<>> verified_kinds_;
    bool has_statements_{false};
    std::map<int, ClaimRecord> claims_;
};

[[nodiscard]] std::expected<UniqueFd, std::string> connect_tcp(const std::string_view host, const std::string_view port) {
    std::uint16_t port_num = 0;
    auto [ptr, ec] = std::from_chars(port.data(), port.data() + port.size(), port_num);
    if (ec != std::errc{} || port_num == 0) {
        return std::unexpected("Invalid port number");
    }

    std::string host_s(host);
    if (host_s == "localhost") {
        host_s = "127.0.0.1";
    }

    sockaddr_in addr4{};
    addr4.sin_family = AF_INET;
    addr4.sin_port   = htons(port_num);
    if (::inet_pton(AF_INET, host_s.c_str(), &addr4.sin_addr) == 1) {
        UniqueFd sock_fd(::socket(AF_INET, SOCK_STREAM, 0));
        if (!sock_fd) {
            return std::unexpected("socket(AF_INET) failed");
        }
        if (::connect(sock_fd.get(), reinterpret_cast<const sockaddr*>(&addr4), sizeof(addr4)) == 0) {
            return sock_fd;
        }
        return std::unexpected("IPv4 connection failed");
    }

    sockaddr_in6 addr6{};
    addr6.sin6_family = AF_INET6;
    addr6.sin6_port   = htons(port_num);
    if (::inet_pton(AF_INET6, host_s.c_str(), &addr6.sin6_addr) == 1) {
        UniqueFd sock_fd(::socket(AF_INET6, SOCK_STREAM, 0));
        if (!sock_fd) {
            return std::unexpected("socket(AF_INET6) failed");
        }
        if (::connect(sock_fd.get(), reinterpret_cast<const sockaddr*>(&addr6), sizeof(addr6)) == 0) {
            return sock_fd;
        }
        return std::unexpected("IPv6 connection failed");
    }

    return std::unexpected("Invalid IP address format");
}

}

int main(const int argc, char* argv[]) {
    std::ios::sync_with_stdio(false);

    const std::string host = (argc > 1) ? argv[1] : "127.0.0.1";
    const std::string port = (argc > 2) ? argv[2] : "9999";

    auto res = detail::connect_tcp(host, port);
    if (!res) {
        std::cerr << "Error: " << res.error() << '\n';
        return 1;
    }

    const detail::UniqueFd net_fd = std::move(*res);
    detail::DiscoSolver solver(net_fd.get(), net_fd.get());

    std::array<pollfd, 2> fds{{
        {.fd = net_fd.get(), .events = POLLIN, .revents = 0},
        {.fd = STDIN_FILENO, .events = POLLIN, .revents = 0}
    }};

    std::string server_buf;
    std::string stdin_buf;
    std::array<char, 4096> chunk{};

    while (true) {
        const int ready = ::poll(fds.data(), fds.size(), -1);
        if (ready < 0) {
            if (errno == EINTR) {
                continue;
            }
            break;
        }

        if (fds[0].revents & (POLLIN | POLLHUP)) {
            const ssize_t n = ::read(net_fd.get(), chunk.data(), chunk.size());
            if (n < 0) {
                if (errno == EINTR) {
                    continue;
                }
                break;
            }
            if (n == 0) {
                if (!server_buf.empty()) {
                    solver.process_server_line(server_buf);
                }
                std::cout << "\n\033[1;33m[Connection closed by remote host]\033[0m\n";
                break;
            }
            server_buf.append(chunk.data(), static_cast<std::size_t>(n));
            std::size_t pos = 0;
            while ((pos = server_buf.find('\n')) != std::string::npos) {
                std::string line = server_buf.substr(0, pos);
                if (!line.empty() && line.back() == '\r') {
                    line.pop_back();
                }
                solver.process_server_line(line);
                server_buf.erase(0, pos + 1);
            }
        }

        if (fds[1].revents & POLLIN) {
            const ssize_t n = ::read(STDIN_FILENO, chunk.data(), chunk.size());
            if (n < 0) {
                if (errno == EINTR) {
                    continue;
                }
                break;
            }
            if (n > 0) {
                stdin_buf.append(chunk.data(), static_cast<std::size_t>(n));
                std::size_t pos = 0;
                while ((pos = stdin_buf.find('\n')) != std::string::npos) {
                    std::string line = stdin_buf.substr(0, pos);
                    if (!line.empty() && line.back() == '\r') {
                        line.pop_back();
                    }
                    solver.handle_user_input(line);
                    stdin_buf.erase(0, pos + 1);
                }
            }
        }
    }

    return 0;
}