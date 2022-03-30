//
// Created by tk on 30/03/2022.
//

#include "../include/Controller.h"



namespace util {
    template<typename ReturnType, typename... Args>
    struct function_traits_defs {
        static constexpr size_t arity = sizeof...(Args);

        using result_type = ReturnType;

        template<size_t i>
        struct arg {
            using type = typename std::tuple_element<i, std::tuple<Args...>>::type;
        };
    };

    template<typename T>
    struct function_traits_impl;

    template<typename ReturnType, typename... Args>
    struct function_traits_impl<ReturnType(Args...)>
            : function_traits_defs<ReturnType, Args...> {
    };

    template<typename ReturnType, typename... Args>
    struct function_traits_impl<ReturnType(*)(Args...)>
            : function_traits_defs<ReturnType, Args...> {
    };

    template<typename ClassType, typename ReturnType, typename... Args>
    struct function_traits_impl<ReturnType(ClassType::*)(Args...)>
            : function_traits_defs<ReturnType, Args...> {
    };

    template<typename ClassType, typename ReturnType, typename... Args>
    struct function_traits_impl<ReturnType(ClassType::*)(Args...) const>
            : function_traits_defs<ReturnType, Args...> {
    };

    template<typename ClassType, typename ReturnType, typename... Args>
    struct function_traits_impl<ReturnType(ClassType::*)(Args...) const &>
            : function_traits_defs<ReturnType, Args...> {
    };

    template<typename ClassType, typename ReturnType, typename... Args>
    struct function_traits_impl<ReturnType(ClassType::*)(Args...) const &&>
            : function_traits_defs<ReturnType, Args...> {
    };

    template<typename ClassType, typename ReturnType, typename... Args>
    struct function_traits_impl<ReturnType(ClassType::*)(Args...) volatile>
            : function_traits_defs<ReturnType, Args...> {
    };

    template<typename ClassType, typename ReturnType, typename... Args>
    struct function_traits_impl<ReturnType(ClassType::*)(Args...) volatile &>
            : function_traits_defs<ReturnType, Args...> {
    };

    template<typename ClassType, typename ReturnType, typename... Args>
    struct function_traits_impl<ReturnType(ClassType::*)(Args...) volatile &&>
            : function_traits_defs<ReturnType, Args...> {
    };

    template<typename ClassType, typename ReturnType, typename... Args>
    struct function_traits_impl<ReturnType(ClassType::*)(Args...) const volatile>
            : function_traits_defs<ReturnType, Args...> {
    };

    template<typename ClassType, typename ReturnType, typename... Args>
    struct function_traits_impl<ReturnType(ClassType::*)(Args...) const volatile &>
            : function_traits_defs<ReturnType, Args...> {
    };

    template<typename ClassType, typename ReturnType, typename... Args>
    struct function_traits_impl<ReturnType(ClassType::*)(Args...) const volatile &&>
            : function_traits_defs<ReturnType, Args...> {
    };

    template<typename T, typename V = void>
    struct function_traits
            : function_traits_impl<T> {
    };

    template<typename T>
    struct function_traits<T, decltype((void) &T::operator())>
            : function_traits_impl<decltype(&T::operator())> {
    };

    template<size_t... Indices>
    struct indices {
        using next = indices<Indices..., sizeof...(Indices)>;
    };
    template<size_t N>
    struct build_indices {
        using type = typename build_indices<N - 1>::type::next;
    };
    template<>
    struct build_indices<0> {
        using type = indices<>;
    };
    template<size_t N>
    using BuildIndices = typename build_indices<N>::type;

    namespace details {
        template<typename FuncType,
                typename VecType,
                size_t... I,
                typename Traits = function_traits<FuncType>,
                typename ReturnT = typename Traits::result_type>
        ReturnT do_call(FuncType &func,
                        VecType &args,
                        indices<I...>) {
            assert(args.size() >= Traits::arity);
            return func(args[I]...);
        }
    }  // namespace details

    template<typename FuncType,
            typename VecType,
            typename Traits = function_traits<FuncType>,
            typename ReturnT = typename Traits::result_type>
    ReturnT unpack_caller(FuncType &func,
                          VecType &args) {
        return details::do_call(func, args, BuildIndices<Traits::arity>());
    }
}
Controller::Controller() {
    table = RuleTable();
}

void Controller::add_rule(std::vector<string> args){
    std::cout << "Test!!!!!!!!!!!!! " << args.size() << std::endl;
    int j = util::unpack_caller((table.AddRule), args);

}
void Controller::remove_rule(std::vector<string> args){

}
void Controller::show_rules(std::vector<string> args){

}
void Controller::start(std::vector<string> args){

}
void Controller::stop(std::vector<string> args){

}

void Controller::run(){

    std::vector<std::string> args;
    do
    {
        args.clear();
        std::cout << "Enter command: " << std::endl;
        std::string rawInput;
        while (true) {
            std::cin >> rawInput;
            args.push_back(rawInput);
            if (std::cin.peek() == '\n')
            {
                break;
            }
        }
        if(args[0] == "quit"){
            break;
        }
        if ( auto it{ cmd_map.find( args[0] ) }; it != cmd_map.end())
        {
            if(args.size() - 1 == cmd_map.at(args[0]).first){
                cmd_map.at(args[0]).second(std::vector<string>(args.begin()+1, args.end()));
            }
        }
        std::string selection = args[0], cmd;
    }while(true);
}